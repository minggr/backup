#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

#define HOST "lkp-os"
#define USER "perf"
#define PASSWD "perf"
#define DB "ymz_stp_results_internal"

#define HOST_ID 1
#define TASK_ID 2
#define KERNEL_ID 3

#define HOST_ID_QUERY_STR "select id from hosts where name=\"%s\""
#define TASK_ID_QUERY_STR "select id from tests where title=\"%s\""
#define KERNEL_ID_QUERY_STR "select id from kernels where ptesting=0 and name=\"%s\""
#define VALUE_QUERY_STR "select raw_result from results where host_id=%d and test_id=%d and kernel_id=%d"
#define TASK_QUERY_STR "select title, invert from tests where ptesting=0 and visible=1 order by title"

#define TASK_LENGTH 256

#define DEBUG 0
#if DEBUG
#define debug_printf printf
#else
#define debug_printf(format, ...)
#endif

char *host_list[] = {
	/* Tulsa */
	"lkp-tulsa01-x8664",

	/* Tigerton */
	"lkp-tt01-x8664", "lkp-tt02-x8664",

	/* Stockley */
	"lkp-st01-x8664", "lkp-st02-x8664",

	/* Nehalem */
	"lkp-ne02", "lkp-nex03", "otc-greencity-09",

	/* Westmere */
	"linux-ws01", "lkp-ne01", "lkp-ws02",

	/* IA64 */
	"lkp-h01", "lkp-mb01", 

	/* Terminator */
	NULL
};

char *skip_task_list[] = {
	"VolanoMark",
	"prx-", "volano-8", "volano-64", "volano-128",
	"tio_thr-noop_", "tio_thr-as_", "netperf2",
	"idlebench",
	"hackbenchpth75", "hackbenchpth50", "hackbenchpth300", "hackbenchpth200",
	"hackbench75", "hackbench50", "hackbench300", "hackbench200",
	"aim7-jtt-2000",
	"dbench-noop", "dbench-deadline", "dbench-cfq", "dbench-as",
	NULL 
};

int skip_task(char *name)
{
	char **task;

	task = skip_task_list;

	while (*task) {
		if (strstr(name, *task))
			return 1;
		task++;
	}

	return 0;
}


typedef struct lkp_task {
	char name[TASK_LENGTH];
	int invert;

	struct lkp_task *next;
} LKP_TASK;

typedef struct lkp_data {
	char host[56];
	char task[256];
	char prev_kernel[56];
	char this_kernel[56];
	float prev_value;
	float this_value;
	float pct;

	struct lkp_data *next;
} LKP_DATA;

LKP_DATA *data_list = NULL;
LKP_TASK *task_list = NULL;

MYSQL mysql;
MYSQL *conn;
char query_str[1024];

/* Connect to LKP database */
int lkp_connect()
{
	if (!mysql_init(&mysql))
		return 0;

	conn = mysql_real_connect(&mysql, HOST, USER, PASSWD, DB, 0, NULL, 0);
	if (!conn) {
	debug_printf("Connect to LKP database fail: %s\n", mysql_error(&mysql));
	debug_printf("HOST: %s, USER: %s, PASSWORD: %s, DB: %s\n",
			HOST, USER, PASSWD, DB);
		return 0;
	}

	return 1;
}

/* Close database connection */
void lkp_close()
{
	mysql_close(conn);
}

void lkp_linktask(LKP_TASK *task)
{
	if (!task)
		return;

	task->next = task_list;
	task_list = task;
}

void lkp_freealltask()
{
	LKP_TASK *next_task;

	while (task_list) {
		next_task = task_list->next;
		free(task_list);
		task_list = next_task;
	}
}

void lkp_getalltask()
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	LKP_TASK *task;
	unsigned long *length;
	char invert[2] = {0, 0};

	sprintf(query_str, TASK_QUERY_STR);

	if (mysql_query(conn, query_str)) {
		debug_printf("Query failed: %s, %s\n", query_str, mysql_error(conn));
		return;
	}

	result = mysql_store_result(conn);
	if (!result) {
	debug_printf("Query failed: %s, %s\n", query_str, mysql_error(conn));
		return;
	}

	while (row = mysql_fetch_row(result)) {
		length = mysql_fetch_lengths(result);
		task = malloc(sizeof(LKP_TASK));
		if (!task) {
			debug_printf("No memory\n");
			continue;
		}
		strncpy(task->name, row[0], length[0]);
		task->name[length[0]] = 0;
		if (skip_task(task->name))
			continue;
		strncpy(invert, row[1], 1);
		task->invert = atoi(invert);
		lkp_linktask(task);
	}
	
	mysql_free_result(result);
}

int lkp_getid(char *name, int type)
{
	MYSQL_RES *result;
	MYSQL_ROW row;

	switch(type) {
	case HOST_ID:
		sprintf(query_str, HOST_ID_QUERY_STR, name);
		break;

	case TASK_ID:
		sprintf(query_str, TASK_ID_QUERY_STR, name);
		break;

	case KERNEL_ID:
		sprintf(query_str, KERNEL_ID_QUERY_STR, name);
		break;

	default:
	debug_printf("Unknown ID type: %d\n", type);
		return -1;
	}

	if (mysql_query(conn, query_str)) {
	debug_printf("Query failed: %s, %s\n", query_str, mysql_error(conn));
		return -1;
	}

	result = mysql_store_result(conn);
	if (!result) {
	debug_printf("Query failed: %s, %s\n", query_str, mysql_error(conn));
		return -1;
	}

	if (mysql_num_rows(result) > 1) {
	debug_printf("Unexpected: ID query result more than 1 row: %s\n",
			query_str);
		return -1;
	}

	row = mysql_fetch_row(result);
	if (!row || !row[0]) {
	debug_printf("ID query result is NULL: %s\n", query_str);
		return -1;
	}

	mysql_free_result(result);

	return atoi(row[0]);
}

/*
 * Get value correspond to "host", "task" and "kernel"
 * For example, host "lkp-ne01", task "oltp", kernel "2.6.33-rc3"
 */
int lkp_getvalue(char *host, char *task, char *kernel, float **value)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	int num;
	unsigned long *length;
	char *v_str;
	float *v;
	int i = 0;

	int host_id = lkp_getid(host, HOST_ID);
	int task_id = lkp_getid(task, TASK_ID);
	int kernel_id = lkp_getid(kernel, KERNEL_ID);

	if (host_id < 0 || task_id < 0 || kernel_id < 0) {
	debug_printf("Some ID(s) is not correct: host %s(%d), task %s(%d), kernel %s(%d)\n",
			host, host_id, task, task_id, kernel, kernel_id);

		return 0;
	}

	sprintf(query_str, VALUE_QUERY_STR, host_id, task_id, kernel_id);

	if (mysql_query(conn, query_str)) {
	debug_printf("Query failed: %s, %s\n", query_str, mysql_error(conn));
		return 0;
	}

	result = mysql_store_result(conn);
	if (!result) {
	debug_printf("Query failed: %s, %s\n", query_str, mysql_error(conn));
		return 0;
	}

	num = mysql_num_rows(result);	
	if (!num)
		return 0;
	v = malloc(sizeof(float) * num);

	while (row = mysql_fetch_row(result)) {
		length = mysql_fetch_lengths(result);	
		if (!length) {
			v[i++] = 0;
			continue;
		}
		v_str = malloc(*length + 1);
		strncpy(v_str, row[0], *length);
		v_str[*length] = 0;
		v[i++] = atof(v_str);
		free(v_str);
	}
	
	mysql_free_result(result);

	*value = v;

	return num;
}

float lkp_getavgvalue(char *host, char *task, char *kernel)
{
	float *value, v = 0;
	int count, i;

	count = lkp_getvalue(host, task, kernel, &value);
	if (!count)
		return -1;

	for (i = 0; i < count; i++) {
		v += value[i];
	}

	v = v / count;

	free(value);

	return v;
}

void lkp_linkdata(LKP_DATA *data)
{
	if (!data)
		return;

	data->next = data_list;
	data_list = data;
}

void lkp_freealldata()
{
	LKP_DATA *next_data;

	while (data_list) {
		next_data = data_list->next;
		free(data_list);
		data_list = next_data;
	}
}

void lkp_getalldata(char *prev_kernel, char *this_kernel)
{
	char **host;
	LKP_TASK *task;
	float prev_value, this_value;
	LKP_DATA *data;

	host = host_list;

	while (*host) {
		task = task_list;

		while (task) {
			prev_value = lkp_getavgvalue(*host, task->name, prev_kernel);
			this_value = lkp_getavgvalue(*host, task->name, this_kernel);

			if (prev_value < 0 || this_value < 0) {
				task = task->next;
				continue;
			}

			data = malloc(sizeof(LKP_DATA));
			if (!data) {
			debug_printf("No memory\n");
				task = task->next;
				continue;
			}

			strcpy(data->host, *host);
			strcpy(data->task, task->name);
			strcpy(data->prev_kernel, prev_kernel);
			strcpy(data->this_kernel, this_kernel);
			data->prev_value = prev_value;
			data->this_value = this_value;
			data->pct = (this_value - prev_value) / prev_value;
			if (task->invert)
				data->pct = -(data->pct);

			lkp_linkdata(data);

			task = task->next;
		}
		host++;
	}
}

LKP_DATA* lkp_searchdata(char *host, char *task)
{
	LKP_DATA *data;

	data = data_list;

	while (data) {
		if (!strcmp(data->host, host) && !strcmp(data->task, task))
			return data;

		data = data->next;
	}

	return NULL;
}

void lkp_printhost(FILE *csv)
{
	char **host;

	host = host_list;

	fprintf(csv, ",\n");
	while (*host) {
		fprintf(csv, ",%s", *host);
		host++;
	}
	fprintf(csv, "\n");
}

void lkp_csv(char *prev_kernel, char *this_kernel)
{
	char **host;
	LKP_TASK *task;
	float prev_value, this_value;
	LKP_DATA *data;
	float pct;
	FILE *csv;
	char file_name[1024];
	int i = 0;

	sprintf(file_name, "%s_compared_to_%s.csv", this_kernel, prev_kernel);
	csv = fopen(file_name, "w");
	if (!csv) {
	debug_printf("open file error\n");
		return;
	}	
	fprintf(csv, "v%s,compared to,v%s\n,\n", this_kernel, prev_kernel);

	task = task_list;

	while (task) {
		if (!(i++ % 20))
			lkp_printhost(csv);

		host = host_list;
		fprintf(csv, "%s,", task->name);

		while (*host) {
			data = lkp_searchdata(*host, task->name);	
			if (data)
				fprintf(csv, "%f%%,", data->pct*100);
			else
				fprintf(csv, ",");
			host++;
		}

		fprintf(csv, "\n");

		task = task->next;
	}

	fclose(csv);
}

int main(int argc, char *argv[])
{
	float *value, v = 0;
	int count, i;
	char *prev_kernel;
	char *this_kernel;

	if (argc < 3) {
		printf("Usage: %s <old_kernel> <new_kernel>\n", argv[0]);
		printf("For example: %s 2.6.33-rc2 2.6.33-rc3\n", argv[0]);

		exit(0);
	}

	prev_kernel = argv[1];
	this_kernel = argv[2];

	if (!lkp_connect()) {
		return -1;
	}

	lkp_getalltask();
	lkp_getalldata(prev_kernel, this_kernel);

	lkp_csv(prev_kernel, this_kernel);

	lkp_freealldata();
	lkp_freealltask();

	lkp_close();

	return 0;
}

all: lkp_csv
CFLAGS=-g -c -I/usr/include/mysql
LDFLAGS=-L/usr/lib64/mysql -lmysqlclient

lkp_csv.o: lkp_csv.c
	gcc ${CFLAGS} lkp_csv.c

lkp_csv: lkp_csv.o
	gcc -o lkp_csv ${LDFLAGS} lkp_csv.o

clean:
	rm -f lkp_csv.o lkp_csv

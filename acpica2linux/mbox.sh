#!/bin/sh

git_patch_dir=`pwd`/mbox
mkdir $git_patch_dir > /dev/null 2>&1
rm -rf $git_patch_dir/*

cd /usr/src/linux-2.6

old_release=2b12a4c524812fb3f6ee590a02e65b95c8c32229
new_release=HEAD

commits="`git-rev-list --reverse $old_release..$new_release`"
commits_count="`git-rev-list --reverse $old_release..$new_release | wc -l`"
total_patches=$commits_count

digit=1
while [ $commits_count -gt 9 ] ; do
	(( commits_count = commits_count / 10 ))
	(( digit = digit + 1 ))	
done

mbox=R07_29_08.mbox
mbox=${git_patch_dir}/$mbox
> $mbox

num=0

for commit in $commits ; do
	(( num = num + 1 ))

	#patch file name: 01.patch 02.patch ...
	filename=`printf "%.${digit}d" $num`	
	git show --pretty=email -p --stat $commit > ${git_patch_dir}/${filename}.patch

	sed -i s/"From: Lin Ming <ming.m.lin@intel.com>"/"From: Bob Moore <robert.moore@intel.com>"/ ${git_patch_dir}/${filename}.patch	
	sed -i s/"Subject: \[PATCH\]"/"Subject: \[PATCH $filename\/$total_patches\]"/ ${git_patch_dir}/${filename}.patch
	cat ${git_patch_dir}/${filename}.patch >> $mbox
	echo >> $mbox
done


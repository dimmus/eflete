#!/bin/bash

NEWS_FILE=NEWS.new
CONFIGURE=configure.ac

function line_print {
	eval "printf '%.0s$2' {1..$1}"
}

function list_print {
	oldIFS="$IFS"
	IFS=$'\n'
	for string in $1
   	do
		echo -e "$2$string"
	done
	IFS="$oldIFS"
}

function update_news {
	echo -e ">>> Update NEWS file"
	#exec 3>&1 4>&2
	#exec 1>$NEWS_FILE 2>&1

	#get the last release version
	tmp=(`sed -n 2p NEWS`)
	last_version=${tmp[1]}

	#make header
	title="Eflete $1"
	tlen=${#title}
	line=$(line_print $tlen '=')
	header="$line\n$title\n$line"
	echo -e $header > $NEWS_FILE
	echo -e "" >> $NEWS_FILE
	since="Changes since $last_version"
	echo -e $since >> $NEWS_FILE
	echo -e "$(line_print ${#since} '-')" >> $NEWS_FILE

	#get the features list
	features=`git log --since="$last_version" --grep="@feature" --format=%s`
	if [ -n "$features" ]; then
		echo -e "" >> $NEWS_FILE
		echo -e "Features:" >> $NEWS_FILE
		list_print "$features" "   * " >> $NEWS_FILE
	fi

	#get the fixes list
	fixes=`git log --since="$last_version" --grep="@fix" --format=%s`
	if [ -n "$fixes" ]; then
		echo -e "" >> $NEWS_FILE
		echo -e "Fixes:" >> $NEWS_FILE
		list_print "$fixes" "   * " >> $NEWS_FILE
	fi

	#print to user latest changes
	cat $NEWS_FILE
	#get the previos change history and to result file
	echo -e "" >> $NEWS_FILE
	sed -n '5,$p' NEWS >> $NEWS_FILE

	#trap 'exec 2>&4 1>&3' 0 1 2 3

	echo -e ""
	read -p "Do you want to edit NEWS? [Y/n] " choice
	case "$choice" in
		y|Y ) vim $NEWS_FILE ;;
		* ) ;;
	esac
	
	mv $NEWS_FILE NEWS
	
	read -p "Do you want to commit the updated NEWS? [Y/n] " choice
	case "$choice" in
		y|Y ) commit_NEWS $1 ;;
		* ) ;;
	esac
}

function commit_NEWS {
	git add NEWS
	git commit -m "Update NEWS for version $1"
	echo -e ""
	git log -1
}

function commit_VERSION {
	git add $CONFIGURE
	git commit -m "Update configure for version $1"
	echo -e ""
	git log -1
}

function spinner {
	PID="$1"
	i=1
	sp_chars="-\|/"
	echo -n " "
	while [ -d /proc/$PID ]; do
		printf "\b${sp:i++%${#sp}:1}"
	done
}

function check_build {
	git clean -dxf > /tmp/git_clean.log
	./autogen.sh
	make
	if [ $? -ne 0 ]; then
		echo -e ""
		echo -e "ERROR: Build is failsed."
		exit 1;
	fi

	echo -e ">>> Build success"
}

function check_UTC {
	make check
	if [ $? -ne 0 ]; then
		echo -e ""
		echo -e "ERROR: Build is failsed."
		exit 1;
	fi

	echo -e ">>> UTC success"
}

function update_version {
	echo -e ""
	echo -e ">>> Update version in configure file"
	oldIFS="$IFS"
	IFS='.'
	array=( $1 )
	major="${array[0]}"
	minor="${array[1]}"
	micra="${array[*]:2}"
	IFS="$oldIFS"

	sed "1s/.*/EFLETE_VERSION([$major], [$minor], [$micra], [release])/g" $CONFIGURE > "$CONFIGURE.new"
	mv "$CONFIGURE.new" $CONFIGURE

	echo -e ""
	head $CONFIGURE
	echo -e ""
	read -p "Do you want to edit $CONFIGURE? [Y/n] " choice
	case "$choice" in
		y|Y ) vim $CONFIGURE ;;
		* ) ;;
	esac
	
	read -p "Do you want to commit the updated version? [Y/n] " choice
	case "$choice" in
		y|Y ) commit_VERSION $1 ;;
		* ) ;;
	esac
}

function create_tag {
	git tag -a "$1" -m "v$1"
}

function create_tarballs {
	read -p "Type the path to save tarballs: " path
	if [ -z $path ]; then
		path=$PWD"/eflete-tarballs.$1"
	fi
	mkdir -p $path
	echo -e $path
	make dist
	mv eflete-$1* $path
	cd $path
	md5sum * > "eflete-$1.md5sum"
}

#START HERE
if [ -z $1 ]; then
	echo -e "ERROR: wrong version"
	echo -e "Please set the release version"
	echo -e ""
	echo -e "Usage: $0 VERSION"
	exit 1
fi

update_news $1
update_version $1
create_tag
check_build
check_UTC
create_tarballs $1

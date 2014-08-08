#!/bin/bash

BRANCH=HEAD
if [ ! "$1" == "" ]; then
  BRANCH=$1
fi
echo "Rewriting branch $BRANCH"

git filter-branch -f --commit-filter '
	GIT_COMMITTER_NAME="Wayne Piekarski";
	GIT_AUTHOR_NAME="Wayne Piekarski";
	GIT_COMMITTER_EMAIL="wayne@tinmith.net";
	GIT_AUTHOR_EMAIL="wayne@tinmith.net";
	git commit-tree "$@";
' $BRANCH

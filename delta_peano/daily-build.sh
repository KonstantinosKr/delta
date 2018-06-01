folders=(delta)

deltaVersion="Delta-$(git log --format="%h" -n 1 .)"

tarName="$deltaVersion.tar.gz"

echo $tarName

tar --exclude-vcs --exclude=*.o -czvf  $tarName ${folders[*]} ${files[*]}


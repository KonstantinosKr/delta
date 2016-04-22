sed "s,dummy,${project},g" .cproject > .cproject_tmp
mv .cproject_tmp .cproject
sed "s,dummy,${project},g" .project > .project_tmp
mv .project_tmp .project

# 5830hw

#chmod everything
chmod u+x *.sh
git reset --hard ; git pull; chmod u+x *.sh
git add . ; git commit -m "limit generate to first 4" ; git push

#compile and run gen
./compile_generator.sh
./input_generator

git reset --hard ; git pull; chmod u+x *.sh; ./compile.sh ; ./runcu.sh

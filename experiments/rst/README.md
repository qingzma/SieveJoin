# run experiments

```
./rst.sh 2>&1 | tee rst.log
cat rst.log | grep "time cost:" > rst_time.txt
cat rst.log | grep "(?<=time cost: )[0-9]\d*(\.*\d*)"  > rst_time.log
```

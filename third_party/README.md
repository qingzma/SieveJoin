`cd third_party/tpch_2_17_0/`

`make -f MakeFile`

`cp dbgen qgen dists.dss ../../data/tpch/`

`cd ../../data/tpch/`

`./dbgen -s 0.1 `

`mv *.tbl x0.1`

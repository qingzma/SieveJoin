#ifndef QJOIN_TABLE_TPCH_SCHEMA_COLUMNS_H
#define QJOIN_TABLE_TPCH_SCHEMA_COLUMNS_H

namespace qjoin {
// table CUSTOMER
static const int C_CUSTKEY = 0;
static const int C_NAME = 1;
static const int C_ADDRESS = 2;
static const int C_NATIONKEY = 3;
static const int C_PHONE = 4;
static const int C_ACCTBAL = 5;
static const int C_MKTSEGMENT = 6;
static const int C_COMMENT = 7;

// table LINEITEM
static const int L_ORDERKEY = 0;
static const int L_PARTKEY = 1;
static const int L_SUPPKEY = 2;
static const int L_LINENUMBER = 3;
static const int L_QUANTITY = 4;
static const int L_EXTENDEDPRICE = 5;
static const int L_DISCOUNT = 6;
static const int L_TAX = 7;
static const int L_RETURNFLAG = 8;
static const int L_LINESTATUS = 9;
static const int L_SHIPDATE = 10;
static const int L_COMMITDATE = 11;
static const int L_RECEIPTDATE = 12;
static const int L_SHIPINSTRUCT = 13;
static const int L_SHIPMODE = 14;
static const int L_COMMENT = 15;

// table NATION
static const int N_NATION_KEY = 0;
static const int N_NATION_NAME = 1;
static const int N_REGION_KEY = 2;
static const int N_COMMENT = 3;

// table ORDERS
static const int O_ORDERKEY = 0;
static const int O_CUSTKEY = 1;
static const int O_ORDERSTATUS = 2;
static const int O_TOTALPRICE = 3;
static const int O_ORDERDATE = 4;
static const int O_ORDERPRIORITY = 5;
static const int O_CLERK = 6;
static const int O_SHIPPRIORITY = 7;
static const int O_COMMENT = 8;

// table PART
static const int P_PARTKEY = 0;
static const int P_NAME = 1;
static const int P_MFGR = 2;
static const int P_BRAND = 3;
static const int P_TYPE = 4;
static const int P_SIZE = 5;
static const int P_CONTAINER = 6;
static const int P_RETAILPRICE = 7;
static const int P_COMMENT = 8;

// table PARTSUPP
static const int PS_PARTKEY = 0;
static const int PS_SUPPKEY = 1;
static const int PS_AVAILQTY = 2;
static const int PS_SUPPLYCOST = 3;
static const int PS_COMMENT = 4;

// table region
static const int R_REGIONKEY = 0;
static const int R_NAME = 1;
static const int R_COMMENT = 2;

// table supplier
static const int S_SUPPKEY = 0;
static const int S_NAME = 1;
static const int S_ADDRESS = 2;
static const int S_NATIONKEY = 3;
static const int S_PHONE = 4;
static const int S_ACCTBAL = 5;
static const int S_COMMENT = 6;
}  // namespace qjoin

#endif  // QJOIN_TABLE_TPCH_SCHEMA_COLUMNS_H
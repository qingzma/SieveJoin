// QX - Suppliers and customers in the same nations with the
// purchase history of the customers.
//
// SELECT nationkey, s_suppkey, c_custkey,
// o_orderkey, l_linenumber
// FROM nation, supplier, customer,
// orders, lineitem
// WHERE n_nationkey = s_nationkey
// AND s_nationkey = c_nationkey
// AND c_custkey = o_custkey
// AND o_orderkey = l_orderkey;

#ifndef QJOIN_DB_QX_H_
#define QJOIN_DB_QX_H_
#include "qjoin/options.h"

namespace qjoin {
void Qx(Options& options);
}

#endif  // QJOIN_DB_QX_H_
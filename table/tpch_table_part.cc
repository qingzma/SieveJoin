#include "tpch_table_part.h"

namespace qjoin {
TpchTablePart::TpchTablePart(std::string filename, size_t row_count);

DATABASE_DATA_TYPES TpchTablePart::GetColumnType(int col) const override;
int64_t TpchTablePart::Int64At(const int64_t& row,
                               const int& col) const override;
float TpchTablePart::FloatAt(const int64_t& row, const int& col) const override;

std::shared_ptr<key_index_> TpchTablePart::KeyIndex(int col) override;
const std::vector<db_key_t_>::iterator TpchTablePart::KeyIterator(int col);

void TpchTablePart::buildPartkeyIndex();
}  // namespace qjoin
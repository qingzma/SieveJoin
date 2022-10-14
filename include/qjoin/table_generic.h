#ifndef QJOIN_INCLUDE_QJOIN_TABLE_GENERIC_H_
#define QJOIN_INCLUDE_QJOIN_TABLE_GENERIC_H_

#include "column_extractor.h"
#include "table.h"

namespace qjoin {
using EmptyAugmenter = ColumnExtractor<>;

class TableGenericBase {
 private:
  struct index_element_ {
    db_key_t_ val;
    db_key_t_ idx;
  };

 public:
  std::vector<db_key_t_>& col1_;
  std::vector<db_key_t_>& col2_;

 protected:
  std::shared_ptr<std::vector<db_key_t_>> p_col1_;

  std::shared_ptr<std::vector<db_key_t_>> p_col2_;

  std::vector<index_element_>& col1_index_;
  std::shared_ptr<std::vector<index_element_>> p_col1_index_;
  std::vector<index_element_>& col2_index_;
  std::shared_ptr<std::vector<index_element_>> p_col2_index_;

 public:
  TableGenericBase(bool c12_same)
      : p_col1_(std::make_shared<std::vector<db_key_t_>>()),
        col1_(*p_col1_),
        p_col2_(c12_same ? p_col1_
                         : std::make_shared<std::vector<db_key_t_>>()),
        col2_(*p_col2_),
        p_col1_index_(std::make_shared<std::vector<index_element_>>()),
        col1_index_(*p_col1_index_),
        p_col2_index_(c12_same
                          ? p_col1_index_
                          : std::make_shared<std::vector<index_element_>>()),
        col2_index_(*p_col2_index_) {
    // p_col1_ = std::make_shared<std::vector<db_key_t_>>();
    // p_col2_ = c12_same ? p_col1_ :
    // std::make_shared<std::vector<db_key_t_>>(); p_col1_index_ =
    // std::make_shared<std::vector<index_element_>>(); p_col2_index_ = c12_same
    // ? p_col1_index_
    //                          :
    //                          std::make_shared<std::vector<index_element_>>();
    // col1_ = *p_col1_;
    // col2_ = *p_col2_;
    // col1_index_ = *p_col1_index_;
    // col2_index_ = *p_col2_index_;
  }

  TableGenericBase(std::shared_ptr<std::vector<db_key_t_>> p_col1,
                   std::shared_ptr<std::vector<db_key_t_>> p_col2,
                   std::shared_ptr<std::vector<index_element_>> p_col1_index,
                   std::shared_ptr<std::vector<index_element_>> p_col2_index)
      : p_col1_(p_col1),
        col1_(*p_col1),
        p_col2_(p_col2),
        col2_(*p_col2),
        p_col1_index_(p_col1_index),
        col1_index_(*p_col1_index),
        p_col2_index_(p_col2_index),
        col2_index_(*p_col2_index) {}

  void BuildIndex(bool for_all = false);

  int Size();

  int CountCardinality(int val, int col = 1);

  db_key_t_ Col1Val(int index);
  db_key_t_ Col2Val(int index);
};

template <class AugmenterType>
class TableGenericImpl : public TableGenericBase {
 private:
  typedef decltype(AugmenterType()('|', "")) AuxcolType;
  static const bool hasAugmenter =
      !std::is_same<std::decay_t<AuxcolType>, std::tuple<>>::value;
  std::shared_ptr<std::vector<AuxcolType>> p_auxcols_;
  std::vector<AuxcolType>& auxcols_;

  TableGenericImpl(decltype(p_col1_) p_col1, decltype(p_col2_) p_col2,
                   decltype(p_col1_index_) p_col1_index,
                   decltype(p_col2_index_) p_col2_index,
                   decltype(p_auxcols_) p_auxcols)
      : TableGenericBase(p_col1, p_col2, p_col1_index, p_col2_index),
        p_auxcols_(p_auxcols),
        auxcols_(*p_auxcols) {}

 public:
  TableGenericImpl(std::string input_file, char delim, int col1, int col2,
                   AugmenterType aug = AugmenterType())
      : TableGenericBase(col1 == col2),
        p_auxcols_(std::make_shared<std::vector<AuxcolType>>()),
        auxcols_(*p_auxcols_) {
    std::ifstream f;
    f.open(input_file);
    std::string buffer;
    while (std::getline(f, buffer)) {
      // find col1
      if (col1 >= 0) {
        size_t idx = 0;
        for (int i = 1; i < col1; i++) {
          idx = buffer.find(delim, idx);
          idx++;
        }
        size_t idx_r = buffer.find(delim, idx);
        col1_.push_back(std::stoll(buffer.substr(idx, idx_r - idx)));
      }

      if (col2 >= 0 && col1 != col2) {
        size_t idx = 0;
        for (int i = 1; i < col2; i++) {
          idx = buffer.find(delim, idx);
          idx++;
        }
        size_t idx_r = buffer.find(delim, idx);
        col2_.push_back(std::stoll(buffer.substr(idx, idx_r - idx)));
      }

      if (hasAugmenter) {
        auxcols_.push_back(aug(delim, buffer));
      }
    }
  }
};

using TableGeneric = TableGenericImpl<EmptyAugmenter>;

class TableGenericWrapper : public Table {
 private:
  std::shared_ptr<TableGenericBase> table_;

 public:
  TableGenericWrapper(std::string input_file, char delim, int column1,
                      int column2)
      : table_(new TableGeneric(input_file, delim, column1, column2)){};

  TableGenericWrapper(std::shared_ptr<TableGenericBase> table)
      : table_(table) {}

  int ColCount() { return 2; }
  int64_t RowCount() { return table_->Size(); }

  int64_t Int64At(const int64_t row, const int col) {
    switch (col) {
      case 0:
        return table_->Col1Val(row);
      case 1:
        return table_->Col2Val(row);
      default:
        throw std::runtime_error("unexpected column.");
    }
  }

  DATABASE_DATA_TYPES GetColumnType(int col) {
    assert(col >= 0 && col < ColCount());
    return DATABASE_DATA_TYPES::INT64;
  }

  const std::vector<db_key_t_>::iterator KeyIterator(int col) {
    switch (col) {
      case 0:
        return table_->col1_.begin();
      case 1:
        return table_->col2_.begin();
      default:
        throw std::runtime_error("column does not exist.");
    }
  }
};

}  // namespace qjoin

#endif  // QJOIN_INCLUDE_QJOIN_TABLE_GENERIC_H_
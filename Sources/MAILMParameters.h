#ifndef MAILMPARAMETERS_H_
#define MAILMPARAMETERS_H_

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "IndexFactory.h"
#include "Knowledge.h"
#include "LogBox.h"

struct OptionValue {
  using OptionTypes = std::variant<bool, int, double, std::string>;

 private:
  OptionTypes obj;

 public:
  enum type_id { btype = 0, itype = 1, dtype = 2, stype = 3 };
  OptionValue() : obj(){};

  template <typename T>
  OptionValue(T val) {
    obj = val;
  };
  OptionValue &operator=(const OptionValue &other) {
    if (other.check_type<int>()) {
      obj = other.get<int>();
    } else if (other.check_type<double>()) {
      obj = other.get<double>();
    } else if (other.check_type<std::string>()) {
      obj = other.get<std::string>();
    } else if (other.check_type<bool>()) {
      obj = other.get<bool>();
    } else {
      std::cerr << "irregular option value" << std::endl;
      exit(1);
    }
    return *this;
  };

  template <typename T>
  OptionValue &operator=(const T val) {
    //型チェック
    if (check_type<int>()) {
      if (!std::is_same<int, T>::value) {
        std::cerr << "conflict type data" << std::endl;
      }
    } else if (check_type<double>()) {
      if (!std::is_same<double, T>::value) {
        std::cerr << "conflict type data" << std::endl;
      }
    } else if (check_type<std::string>()) {
      if (!std::is_same<std::string, T>::value) {
        std::cerr << "conflict type data" << std::endl;
      }
    } else if (check_type<bool>()) {
      if (!std::is_same<bool, T>::value) {
        std::cerr << "conflict type data" << std::endl;
      }
    } else {
      std::cerr << "irregular option value" << std::endl;
      exit(1);
    }
    // OptionTypes value = val;
    // obj = value;
    obj = val;
    return *this;
  };

  template <typename T>
  const T &get() const {
    if (std::holds_alternative<T>(obj)) {
      return std::get<T>(obj);
    } else {
      OptionTypes emp = T();
      std::cerr << "invalid get in OptionValue" << std::endl
                << "Value Type ID: " << return_type() << "  Called Type ID: " << emp.index() << std::endl;
      exit(1);
    }
  }

  template <typename T>
  bool check_type() const {
    return std::holds_alternative<T>(obj);
  }

  type_id return_type() const { return static_cast<type_id>(obj.index()); }
};
struct ProgramOption {
 private:
  IndexFactory idx;
  std::map<std::string, int> id;
  std::map<int, OptionValue> val_list;
  std::map<int, std::string> desc_list;
  std::map<int, OptionValue::type_id> type_inf;
  std::set<int> exist;

 public:
  ProgramOption() : idx(), id(), val_list(), desc_list(), type_inf(), exist(){};
  ProgramOption(const ProgramOption &po) {
    idx = po.idx;
    id = po.id;
    val_list = po.val_list;
    desc_list = po.desc_list;
    type_inf = po.type_inf;
    exist = po.exist;
  }

  ProgramOption &operator=(const ProgramOption &po) {
    idx = po.idx;
    id = po.id;
    val_list = po.val_list;
    desc_list = po.desc_list;
    type_inf = po.type_inf;
    exist = po.exist;
    return *this;
  }

  template <typename T>
  static T value() {
    return T();
  }

  template <typename T>
  static T value(T obj) {
    return T(obj);
  }

  int count(std::string str) { return exist.count(id[str]); }

  ProgramOption &add_option() { return *this; }

  ProgramOption &operator()(std::string key, OptionValue val, std::string str) {
    int index = idx.generate();
    id[key] = index;
    val_list[index] = val;
    desc_list[index] = str;
    switch (val.return_type()) {
      case OptionValue::itype:
        type_inf[index] = OptionValue::itype;
        break;
      case OptionValue::dtype:
        type_inf[index] = OptionValue::dtype;
        break;
      case OptionValue::stype:
        type_inf[index] = OptionValue::stype;
        break;
      case OptionValue::btype:
        type_inf[index] = OptionValue::btype;
        break;
      default:
        std::cerr << "unknown type" << std::endl;
        exit(1);
    }
    return *this;
  }
  ProgramOption &operator()(std::string key, std::string str) {
    int index = idx.generate();
    id[key] = index;
    val_list[index] = bool(false);
    desc_list[index] = str;
    type_inf[index] = OptionValue::btype;
    return *this;
  }

  template <typename T>
  const T &get(std::string str) {
    return val_list[id[str]].get<T>();
  }
  void help() {
    std::string help_comment = "HELP:\n";
    for (auto index : id) {
      help_comment += "  " + index.first + "\t" + desc_list[index.second] + "\n";
    }
    std::cout << help_comment << std::endl;
    exit(0);
  }
  void parse(int argc, char *argv[]) {
    int i = 1;
    std::string key = "";
    std::stringstream ss;
    while (i < argc) {
      std::string str(argv[i]);
      std::string option;
      if (str.find("-") != 0 || (str.find("-") == 0 && str.size() < 2) || (str.find("--") == 0 && str.size() < 3)) {
        if (id.count(key) == 0) {
          std::cerr << "unknown option key" << std::endl;
          exit(1);
        } else {
          exist.insert(id[key]);
          OptionValue::type_id ti = type_inf[id[key]];
          bool b;
          try {
            switch (ti) {
              case OptionValue::itype:
                val_list[id[key]] = std::stoi(str);
                break;
              case OptionValue::dtype:
                val_list[id[key]] = std::stod(str);
                break;
              case OptionValue::stype:
                val_list[id[key]] = str;
                break;
              case OptionValue::btype:
                ss << std::boolalpha << str;
                ss >> b;
                ss << std::noboolalpha;
                val_list[id[key]] = b;
                break;
              default:
                std::cerr << "unknown type_id" << std::endl;
                exit(1);
            }
          } catch (...) {
            std::cerr << "Error in ProgramOption::parse . substitution of type[" << ti << "]" << std::endl;
            exit(1);
          }
        }
        key = "";
        i++;
        continue;
      }
      if (key != "") {
        exist.insert(id[key]);
        try {
          val_list[id[key]] = true;
        } catch (...) {
          std::cerr << "Error in ProgramOption::parse . substitution of bool type" << std::endl;
          exit(1);
        }
      }
      if (str.find("--") == 0) {
        std::copy(std::begin(str) + 2, std::end(str), std::back_inserter(option));
      } else {
        std::copy(std::begin(str) + 1, std::end(str), std::back_inserter(option));
      }
      if (option == "h" || option == "help") {
        help();
      }
      if (id.count(option)) {
        key = option;
        i++;
      } else {
        std::cerr << "unknown option" << std::endl;
        exit(1);
      }
    }
    if (key != "") {
      exist.insert(id[key]);
      val_list[id[key]] = true;
    }
  }
};

/*!
 * 実行時引数を解釈して、保持するクラス
 */
class MAILMParameters {
 public:
  // experiment parameters
  int MAX_GENERATIONS;
  double PER_UTTERANCES;
  int RANDOM_SEED;  //

  uint32_t CONTROLS;
  int buzz_length;

  // Execution Values
  int UTTERANCES;
  uint32_t Generation_Counter;  //

  // system parameters
  bool MONO;
  bool ABC;
  bool LILYPOND;
  bool LOGGING;
  bool ANALYZE;

  std::string DICTIONARY_FILE;

  // file parameters
  // file prefix
  std::string FILE_PREFIX;
  std::string DATE_STR;

  // file extentions
  std::string RESULT_EXT;
  std::string LOG_EXT;

  // path
  std::string BASE_PATH;
  std::string RESULT_PATH;

  // file
  std::string LOG_FILE;
  std::string RESULT_FILE;

  ProgramOption spo;

  std::string INPUT_FILE;
  std::string ALIAS_FILE;
  std::string DIC_XML_FILE;
  std::string XML_DIR;
  std::string XML_EXT;
  std::string MONOXML_DIR;
  std::string ABC_DIR;
  std::string ABC_EXT;

  MAILMParameters();
  ~MAILMParameters();

  void set_option(ProgramOption &po);
  std::string to_s(void);
  std::string to_all_s(void);
};

#endif /* MAILMPARAMETERS_H_ */

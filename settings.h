#ifndef __JIG_SETTINGS_H__
#define __JIG_SETTINGS_H__

#include <string>
#include <unordered_map>

namespace jig {

class Settings {
public:
  enum class ValueType {
    UNKNOWN,
    BOOLEAN,
    NUMBER,
    STRING,
  };

  static ValueType getRealValueTypeForValueAsString(const std::string &value);
  static const char *const getValueTypeString(ValueType type);

  static bool convertStringToBoolean(const std::string &str);
  static int convertStringToNumber(const std::string &str);

  Settings() = default;

  template <typename T>
  T get(const std::string &key);

  template <typename T>
  void set(const std::string &key, T value) {
    m_Options[key] = value;
  }

private:
  class Value {
  public:
    Value() : m_Type{ValueType::UNKNOWN} {}
    Value(bool value) : m_Type{ValueType::BOOLEAN} { m_Value.b = value; }
    Value(int value) : m_Type{ValueType::NUMBER} { m_Value.n = value; }
    Value(const char *value) : m_Type{ValueType::STRING} { m_Value.s = value; }

    Value &operator=(bool value);
    Value &operator=(int value);
    Value &operator=(const char *value);

    ValueType getType() const { return m_Type; }
    void setType(ValueType type) { m_Type = type; }

    bool getBoolean() const { return m_Value.b; }
    int getNumber() const { return m_Value.n; }
    const char *getString() const { return m_Value.s; }

  private:
    ValueType m_Type;
    union {
      bool b;
      int n;
      const char *s;
    } m_Value;
  };

  bool get(const std::string &key, bool &value);
  bool get(const std::string &key, int &value);
  bool get(const std::string &key, std::string &value);

  std::unordered_map<std::string, Value> m_Options;
};

} // namespace jig

#endif // __JIG_SETTINGS_H__

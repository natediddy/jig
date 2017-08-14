#ifndef __JIG_STRUTILS_H__
#define __JIG_STRUTILS_H__

#include <string>
#include <vector>

namespace jig {
namespace str {

std::size_t occurs(const std::string &str, char ch);
std::size_t occurs(const std::string &str, const std::string &s);

std::vector<std::size_t> findAllPositions(const std::string &str, char ch);
std::vector<std::size_t> findAllPositions(const std::string &str,
                                          const std::string &s);

void stripAllWhitespace(std::string &str);

bool areEqualIgnoreCase(const std::string &str1, const std::string &str2);

std::string format(const char *fmt, ...);

} // namespace jig
} // namespace jig

#endif // __JIG_STRUTILS_H__

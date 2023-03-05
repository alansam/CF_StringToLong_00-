#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cctype>
#include <climits>
#include <iostream>
#include <iomanip>
#include <stdexcept>

int main(int argc, char const * argv[]) {
  char const * smpl[] = {
    "0", "1", "0xffff 55", "abc",
    "+0xffffffff",
    "-0xffffffff",
    " 0xffffffffffffffff", "-1",
    "-0xffffffffffffffff", "0-1",
    "-0x8000000000000000", "-9223372036854775808",
    "+0x7fffffffffffffff", "+9223372036854775807",
    " 0x800000000000000000",
    " 0x8000000000000001", "-9223372036854775807",
    " 0x7000000000000000", "+8070450532247928832",
    "-0777777", "+0777777",
    "256Q",
  };
  size_t const smpl_sz = sizeof smpl / sizeof *smpl;

  printf("%21ld %21ld\n", LONG_MIN, LONG_MAX);
  for (size_t s_ = 0ul; s_ < smpl_sz; ++s_) {
    char const * nptr = smpl[s_];
    long rslt = 0l;
    char * endchar = NULL;
    int errsv = 0;
    errno = 0;
    rslt = ::strtol(nptr, &endchar, 0);
    errsv = errno;

    ::printf("%21ld, %#18lx, %20p, %20p, %5d, "
             "|%-24s| `%s`\n",
             rslt, rslt, (void *) nptr,
             (void *) endchar, errsv, nptr, endchar);
    if (errsv != 0) {
      char gronk[64];
      sprintf(gronk, "GRONK [%5d]", errsv);
      perror(gronk);
      char const * rer = strerror(errsv);
      char const * ren = strerrorname_np(errsv);
      char const * red = strerrordesc_np(errsv);
      ::fprintf(stderr, " - error msg : %s\n", rer);
      ::fprintf(stderr, " - error name: %s [%d]\n", ren, errsv);
      ::fprintf(stderr, " - error desc: %s\n", red);
    }
  }
  ::putchar('\n');

  std::cout << std::string(60, '~') << '\n';
  for (auto smp : smpl) {
    std::size_t pos {};
    auto rslt = 0l;
    try {
      rslt = std::stol(smp, &pos, 0);
      std::cout << std::setw(24) << std::quoted(smp, '|')
                << std::setw(21) << rslt
                << std::setw(5)  << pos
                << '\n';
    }
    catch (std::invalid_argument const & ex) {
      std::cerr << std::setw(24) << std::quoted(smp, '`')
                << '\n';
      std::cerr << "std::invalid_argument.what(): "
                << ex.what()
                << '\n';
    }
    catch (std::out_of_range const & ex) {
      std::cerr << std::setw(24) << std::quoted(smp, '`')
                << '\n';
      std::cerr << "std::out_of_range.what(): "
                << ex.what()
                << '\n';
    }  }

  return EXIT_SUCCESS;
}

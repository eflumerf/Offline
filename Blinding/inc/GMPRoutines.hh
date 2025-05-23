// Ed Callaghan
// A collection of routines using gmp functions and types
// August 2024

// these functions implement probabilistic encryption and decryption
// of a single bit of information (encapsulated below in an
// unsigned int, ``set''). a fully technical description of the
// scheme is given in Goldwasser & Micali, "Probabilistic encryption,"
// Journal of Computer and System Sciences, 28 (2): 270–299, 1984.
// doi:10.1016/0022-0000(84)90070-9. in particular, the mathematical
// manipulations implemented here are contextualized in Section 6.
//
// That being said, the following wikipedia pages offer an easier
// introduction for non-experts:
// - https://en.wikipedia.org/wiki/Goldwasser%E2%80%93Micali_cryptosystem
// - https://en.wikipedia.org/wiki/Quadratic_residuosity_problem
//
// this implementation operates on arbitrary-sized integers within
// the context of the GNU MultiPrecision system, which also
// contains the number-theoretic functions on which the cryposysytem
// is designed.

#ifndef GMPRoutines_hh
#define GMPRoutines_hh

#include <gmp.h>

namespace mu2e{
  namespace gmp{
    // test if x is a quadratic resiue of N = p * q
    bool is_quadratic_residue(mpz_t x,
                              const mpz_t& p,
                              const mpz_t& q,
                              const mpz_t& N);

    // identify a non-quadratic residue of N = p * q
    void sample_nonresidue(mpz_t x,
                          const mpz_t& p,
                          const mpz_t& q,
                          const mpz_t& N,
                          gmp_randstate_t state);

    // identify x s.t. x == 1 (mod N)
    void sample_unit_modulo(mpz_t x,
                            const mpz_t& N,
                            gmp_randstate_t state);

    // apply Goldwasser-Micali encryption to a single bit
    void gm_encrypt(mpz_t encrypted,
                    unsigned int set,
                    const mpz_t& x,
                    const mpz_t& N,
                    gmp_randstate_t state);

    // apply Goldwasser-Micali decryption for a single bit; N = p * q
    unsigned int gm_decrypt(mpz_t encrypted,
                            const mpz_t& p,
                            const mpz_t& q,
                            const mpz_t& N);
  } // namespace gmp
} // namespace mu2e

#endif


#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List rcpp_hello_world() {

    CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
    NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;
    List z            = List::create( x, y ) ;

    return z ;
}

// [[Rcpp::export]]
int WriteBinaryDosage(const Rcpp::NumericMatrix &x, const int numSubjects, const int numSNPs) {
  const double *y;
  std::vector<unsigned short> z;
  unsigned short *p0, *p1, *p2, *dosage;
  unsigned short z0, z1;
  unsigned ui;
  
  Rcpp::Rcout << x.nrow() << '\t' << x.ncol() << std::endl;
  if (numSubjects * numSNPs != x.nrow()) {
    Rcpp::Rcerr << "Number of rows inconsist with number of subjects and SNPs" << std::endl;
    return 1;
  }
  y = &x[0];
  z.resize(numSubjects * numSNPs * 4);
  for (ui = 0; ui < numSubjects * numSNPs * 4; ++ui) {
    z0 = (unsigned short)(y[ui] * 0xfffe);
    z1 = z0 + 1;
    if (ui == 0 || ui == numSubjects * numSNPs) {
      Rcpp::Rcout << y[ui] << '\t' << z0 << '\t' << z1 << std::endl;
      Rcpp::Rcout << y[ui] - (double)(z0) / 0xfffe << '\t' << (double)(z1) / 0xfffe - y[ui] << std::endl;
    }
    if (y[ui] - (double)(z0 * 0xfffe) > (double)(z1 * 0xfffe) - y[ui])
      z[ui] = z1;
    else
      z[ui] = z0;
  }
  p0 = &z[0];
  p1 = p0 + numSubjects * numSNPs;
  p2 = p1 + numSubjects * numSNPs;
  dosage = p2 + numSubjects * numSNPs;
  
  Rcpp::Rcout << *p0 << '\t' << *p1 << '\t' << *p2 << '\t' << *dosage << std::endl;
  return 0;
}
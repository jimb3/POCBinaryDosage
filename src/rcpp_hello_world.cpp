#include <iostream>
#include <fstream>
#include <Rcpp.h>
using namespace Rcpp;

int WriteFormat1_1(const std::string &basefilename, const unsigned short *dosage, unsigned int numSubjects, unsigned int numSNPs) {
  std::string filename;
  std::ofstream outfile;
  const char header[8] = {'b', 'o', 's', 'e', 0x0, 0x1, 0x0, 0x1};
  
  filename = basefilename + "_1_1.bdosage";
  outfile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outfile.good()) {
    Rcpp::Rcerr << "Unable to open output file" << std::endl;
    return 1;
  }
  outfile.write(header, 8);
  outfile.write((char *)dosage, numSubjects * numSNPs * sizeof(unsigned short));
  
  outfile.close();
  return 0;
}

int WriteFormat1_2(const std::string &basefilename, const unsigned short *p1, const unsigned short *p2, unsigned int numSubjects, unsigned int numSNPs) {
  std::string filename;
  std::ofstream outfile;
  const char header[8] = {'b', 'o', 's', 'e', 0x0, 0x1, 0x0, 0x2};
  unsigned int ui;
  const unsigned short *c1, *c2;
  
  filename = basefilename + "_1_2.bdosage";
  outfile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outfile.good()) {
    Rcpp::Rcerr << "Unable to open output file" << std::endl;
    return 1;
  }
  outfile.write(header, 8);
  c1 = p1;
  c2 = p2;
  for (ui = 0; ui < numSNPs; ++ui, c1 += numSubjects, c2 += numSubjects) {
    outfile.write((char *)c1, numSubjects * sizeof(unsigned short));
    outfile.write((char *)c2, numSubjects * sizeof(unsigned short));
  }
  
  outfile.close();
  return 0;
}

int WriteFormat2_1(const std::string &basefilename, const unsigned short *dosage, unsigned int numSubjects, unsigned int numSNPs) {
  std::string filename;
  std::ofstream outfile;
  const char header[8] = {'b', 'o', 's', 'e', 0x0, 0x2, 0x0, 0x1};
  
  filename = basefilename + "_2_1.bdosage";
  outfile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outfile.good()) {
    Rcpp::Rcerr << "Unable to open output file" << std::endl;
    return 1;
  }
  outfile.write(header, 8);
  outfile.write((char *)dosage, numSubjects * numSNPs * sizeof(unsigned short));
  
  outfile.close();
  return 0;
}

int WriteFormat2_2(const std::string &basefilename, const unsigned short *p1, const unsigned short *p2, unsigned int numSubjects, unsigned int numSNPs) {
  std::string filename;
  std::ofstream outfile;
  const char header[8] = {'b', 'o', 's', 'e', 0x0, 0x2, 0x0, 0x2};
  unsigned int ui;
  const unsigned short *c1, *c2;
  
  filename = basefilename + "_2_2.bdosage";
  outfile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outfile.good()) {
    Rcpp::Rcerr << "Unable to open output file" << std::endl;
    return 1;
  }
  outfile.write(header, 8);
  c1 = p1;
  c2 = p2;
  for (ui = 0; ui < numSNPs; ++ui, c1 += numSubjects, c2 += numSubjects) {
    outfile.write((char *)c1, numSubjects * sizeof(unsigned short));
    outfile.write((char *)c2, numSubjects * sizeof(unsigned short));
  }
  
  outfile.close();
  return 0;
}

// [[Rcpp::export]]
List rcpp_hello_world() {

    CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
    NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;
    List z            = List::create( x, y ) ;

    return z ;
}

// [[Rcpp::export]]
int WriteBinaryDosage(const Rcpp::NumericMatrix &x, const int numSubjects, const int numSNPs, const std::string &filename) {
  const double *y;
  std::vector<unsigned short> z;
  std::vector<unsigned short> zD;
  unsigned short *p0, *p1, *p2, *dosage;
  unsigned short *p0D, *p1D, *p2D, *dosageD;
  unsigned short z0, z1;
  unsigned ui;

  Rcpp::Rcout << filename << std::endl;  
  if (numSubjects * numSNPs != x.nrow()) {
    Rcpp::Rcerr << "Number of rows inconsist with number of subjects and SNPs" << std::endl;
    return 1;
  }
  y = &x[0];
  z.resize(numSubjects * numSNPs * 4);
  zD.resize(numSubjects * numSNPs * 4);
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
    zD[ui] = (unsigned short)(y[ui] * 20000);
  }
  p0 = &z[0];
  p1 = p0 + numSubjects * numSNPs;
  p2 = p1 + numSubjects * numSNPs;
  dosage = p2 + numSubjects * numSNPs;
  
  Rcpp::Rcout << *p0 << '\t' << *p1 << '\t' << *p2 << '\t' << *dosage << std::endl;
  Rcpp::Rcout << '\n' << zD.size() << std::endl;

  p0D = &zD[0];
  p1D = p0D + numSubjects * numSNPs;
  p2D = p1D + numSubjects * numSNPs;
  dosageD = p2D + numSubjects * numSNPs;

  Rcpp::Rcout << *p0D << '\t' << *p1D << '\t' << *p2D << '\t' << *dosageD << std::endl;
  
  WriteFormat1_1(filename, dosage, numSubjects, numSNPs);
  WriteFormat1_2(filename, p1, p2, numSubjects, numSNPs);
  WriteFormat2_1(filename, dosageD, numSubjects, numSNPs);
  WriteFormat2_2(filename, p1D, p2D, numSubjects, numSNPs);
  
  return 0;
}
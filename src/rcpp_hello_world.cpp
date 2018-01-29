#include <iostream>
#include <fstream>
#include <cstring>
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

int WriteFormat3_1(const std::string &basefilename, const unsigned short *dosage, unsigned int numSubjects, unsigned int numSNPs) {
  std::string filename;
  std::ofstream outfile;
  const char header[8] = {'b', 'o', 's', 'e', 0x0, 0x3, 0x0, 0x1};
  
  filename = basefilename + "_3_1.bdosage";
  outfile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outfile.good()) {
    Rcpp::Rcerr << "Unable to open output file" << std::endl;
    return 1;
  }
  outfile.write(header, 8);
  outfile.write((const char *)&numSubjects, sizeof(unsigned int));
  outfile.write((char *)dosage, numSubjects * numSNPs * sizeof(unsigned short));
  
  outfile.close();
  return 0;
}

int WriteFormat3_2(const std::string &basefilename, const unsigned short *dosage, const unsigned short *p0, const unsigned short *p1, const unsigned short *p2, unsigned int numSubjects, unsigned int numSNPs) {
  std::string filename;
  std::ofstream outfile;
  const char header[8] = {'b', 'o', 's', 'e', 0x0, 0x3, 0x0, 0x2};
  std::vector<unsigned short> z;
  const unsigned short *pD, *pP0, *pP1, *pP2;
  unsigned short *pa;
  int numAdded;
  int snpSize;
  unsigned int ui, uj;
  
  filename = basefilename + "_3_2.bdosage";
  outfile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outfile.good()) {
    Rcpp::Rcerr << "Unable to open output file" << std::endl;
    return 1;
  }
  z.resize(4 * numSubjects);
  
  outfile.write(header, 8);
  outfile.write((const char *)&numSubjects, sizeof(unsigned int));
  pD = dosage;
  pP0 = p0;
  pP1 = p1;
  pP2 = p2;
  for (ui = 0; ui < numSNPs; ++ui, pD += numSubjects, pP0 += numSubjects, pP1 += numSubjects, pP2 += numSubjects) {
    std::memmove((char *)&z[0], (char *)pD, numSubjects * sizeof(unsigned short));
    pa = &z[numSubjects];
    numAdded = 0;
    for (uj = 0; uj < numSubjects; ++uj) {
      if (pP0[uj] + pP1[uj] + pP2[uj] != 10000 || (pP0[uj] != 0 && pP2[uj] != 0)) {
        z[uj] |= 0x8000;
        pa[numAdded] = 0x8000 | pP1[uj];
        ++numAdded;
        pa[numAdded] = pP0[uj];
        ++numAdded;
        pa[numAdded] = pP2[uj];
        ++numAdded;
      } else if (pP1[uj] + pP2[uj] + pP2[uj] != z[uj]) {
        z[uj] |= 0x8000;
        pa[numAdded] = pP1[uj];
        ++numAdded;
      }
    }
    numAdded += numSubjects;
    snpSize = numAdded + numAdded;
    Rcpp::Rcout << snpSize << std::endl;
    outfile.write((char *)&snpSize, sizeof(int));
    outfile.write((char *)&z[0], numAdded * sizeof(short));
  }
  Rcpp::Rcout << outfile.tellp() << std::endl;
  
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
  int i;

  if (numSubjects * numSNPs != x.nrow()) {
    Rcpp::Rcerr << "Number of rows inconsist with number of subjects and SNPs" << std::endl;
    return 1;
  }
  y = &x[0];
  z.resize(numSubjects * numSNPs * 4);
  zD.resize(numSubjects * numSNPs * 4);
  for (i = 0; i < numSubjects * numSNPs * 4; ++i) {
    z0 = (unsigned short)(y[i] * 0xfffe);
    z1 = z0 + 1;
    if (y[i] - (double)(z0 * 0xfffe) > (double)(z1 * 0xfffe) - y[i])
      z[i] = z1;
    else
      z[i] = z0;
    zD[i] = (unsigned short)(y[i] * 10000);
  }
  p0 = &z[0];
  p1 = p0 + numSubjects * numSNPs;
  p2 = p1 + numSubjects * numSNPs;
  dosage = p2 + numSubjects * numSNPs;
  
  p0D = &zD[0];
  p1D = p0D + numSubjects * numSNPs;
  p2D = p1D + numSubjects * numSNPs;
  dosageD = p2D + numSubjects * numSNPs;

  WriteFormat1_1(filename, dosage, numSubjects, numSNPs);
  WriteFormat1_2(filename, p1, p2, numSubjects, numSNPs);
  WriteFormat2_1(filename, dosageD, numSubjects, numSNPs);
  WriteFormat2_2(filename, p1D, p2D, numSubjects, numSNPs);
  WriteFormat3_1(filename, dosageD, numSubjects, numSNPs);
  WriteFormat3_2(filename, dosageD, p0D, p1D, p2D, numSubjects, numSNPs);
  
  return 0;
}
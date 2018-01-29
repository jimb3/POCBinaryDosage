#include <iostream>
#include <fstream>
#include <cstring>
#include <Rcpp.h>
using namespace Rcpp;

bool CheckBinaryDosageFileSize(std::ifstream &infile, const int version, const unsigned int nSub, const unsigned int nSNPs) {
  long long expectedSize;
  std::streampos actualSize;
  
  expectedSize = nSub * nSNPs * sizeof(short);
  if (version == 2)
    expectedSize += expectedSize;
  expectedSize += 8;
  infile.seekg(0, std::ios_base::end);
  actualSize = infile.tellg();
  Rcpp::Rcout << "Format:\tX." << version << '\t' << expectedSize << '\t' << actualSize << std::endl;
  if (expectedSize == actualSize)
    return true;
  Rcpp::Rcerr << "Binary dosage file is not of the expected size" << std::endl;
  return false;
}

bool CheckBinaryDosage3FileSize(std::ifstream &infile, const int version, const unsigned int nSub, const unsigned int nSNPs) {
  long long expectedSize;
  int snpSize;
  std::streampos actualSize;
  unsigned int ui;
  
  
  if (version == 1) {
    expectedSize = nSub * nSNPs * sizeof(short) + 12;
    infile.seekg(0, std::ios_base::end);
    actualSize = infile.tellg();
    Rcpp::Rcout << "Format:\t3.1\t" << expectedSize << '\t' << actualSize << std::endl;
    if (expectedSize == actualSize)
      return true;
    Rcpp::Rcerr << "Binary dosage file is not of the expected size" << std::endl;
    return false;
  } else {
    infile.seekg(0, std::ios_base::end);
    actualSize = infile.tellg();
    expectedSize = 12 + 4 * nSNPs;
    infile.seekg(12);
    for (ui = 0; ui < nSNPs; ++ui) {
      infile.read((char *)&snpSize, sizeof(int));
      if (!infile.good()) {
        Rcpp::Rcerr << "Reached end of file:\t" << expectedSize << '\t' << actualSize << '\t' << snpSize << '\t' << infile.tellg() << std::endl;
        return false;
      }
      if (ui < 10)
        Rcpp::Rcout << snpSize << std::endl;
      expectedSize += snpSize;
      infile.seekg(snpSize, std::ios_base::cur);
    }
    if (expectedSize != actualSize) {
      Rcpp::Rcerr << "Binary dosage file is not of the expected size\t" << expectedSize << '\t' << actualSize << std::endl;
      return false;
    }
    Rcpp::Rcout << "Format:\t3.1\t" << expectedSize << '\t' << actualSize << std::endl;
  }

  return true;
}
  
// [[Rcpp::export]]
Rcpp::List GetBinaryDosageFormat(const std::string &binaryDosageFilename, const unsigned int nSub, const unsigned int nSNPs) {
  std::ifstream infile;
  const char magicWord[4] = { 'b', 'o', 's', 'e' };
  char readMagicWord[4];
  char readVersion[4];
  int format, version;
  unsigned int numSubjects, numSNPs;
  Rcpp::List result;
  
  format = 0;
  version = 0;
  result = Rcpp::List::create(Rcpp::Named("success") = false,
                              Rcpp::Named("format") = format,
                              Rcpp::Named("version") = version,
                              Rcpp::Named("errorMessage") = "");

  infile.open(binaryDosageFilename.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!infile.good()) {
    result["errorMessage"] = "Unable to open binary dosage file";
    return result;
  }
  infile.read(readMagicWord, 4);
  if (!infile.good()) {
    infile.close();
    result["errorMessage"] = "Error reading header of binary dosage file";
    return result;
  }
  if (std::memcmp(magicWord, readMagicWord, 4)) {
    infile.close();
    result["errorMessage"] = "File is not a binary dosage file";
    return result;
  }
  infile.read(readVersion, 4);
  if (!infile.good()) {
    infile.close();
    result["errorMessage"] = "Error reading format and version number";
    return result;
  }
  if (readVersion[0] || readVersion[2]) {
    infile.close();
    result["errorMessage"] = "Format and version number error";
    return result;
  }
  format = readVersion[1];
  version = readVersion[3];
  if (format < 1 || format > 4) {
    infile.close();
    result["errorMessage"] = "Unknown format";
    return result;
  }
  if (version < 1 || version > 2) {
    infile.close();
    result["errorMessage"] = "Unknown version";
    return result;
  }
  if (format > 2) {
    infile.read((char *)&numSubjects, sizeof(unsigned int));
    result["numSubjects"] = numSubjects;
    if (format > 3) {
      infile.read((char *)&numSNPs, sizeof(unsigned int));
      result["numSNPs"] = numSNPs;      
    } else {
      CheckBinaryDosage3FileSize(infile, version, nSub, nSNPs);
    }
  } else {
    CheckBinaryDosageFileSize(infile, version, nSub, nSNPs);
  }
  
  result["success"] = true;
  result["format"] = format;
  result["version"] = version;
  infile.close();
  return result;
}

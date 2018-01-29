# hasFamilyID -- TRUE data frame has family ID, FALSE otherwise
# subject -- string matrix with family and subject IDs
# phenotype -- numeric vector containing subject phenotypes
# covariates -- numberic matrix with covariate values
# @export
TestSubjectDataR <- function(subjectData) {
  phenoCol = 2;
  firstCov = 3;
  res <- list(success = FALSE, hasFamilyID = FALSE, subjects = NULL, phenotype = NULL, covariates = NULL)
  if (is.data.frame(subjectData) == FALSE) {
    print("SubjectData is not a data frame")
    return (res)
  }
  if (ncol(subjectData) < 3) {
    print("Not enough columns in subject data frame")
    return (res)
  }
  if (is.character(subjectData[,1]) == FALSE) {
    print("First column of SubjectData must be a vector of strings")
    return (res)
  }
  if (is.character(subjectData[,2]) == TRUE) {
    res[["hasFamilyID"]] = TRUE
    if (ncol(subjectData) < 4) {
      print("Not enough columns in subject data frame")
      return (res)
    }
    phenoCol = 3;
    firstCov = 4;
  }
  if (is.numeric(subjectData[,phenoCol]) == FALSE) {
    print("Phenotype column is not numeric")
    return (res)
  }
  for (i in firstCov:ncol(subjectData)) {
    if (is.numeric(subjectData[,i]) == FALSE) {
      print("Not all covariates are numeric")
      return (res)
    }
  }
  res[["success"]] = TRUE
  if (res[["hasFamilyID"]] == TRUE) {
    res[["subjects"]] = as.matrix(subjectData[,c(1:2)])
  } else {
    res[["subjects"]] = as.matrix(subjectData[,1])
  }
  res[["phenotype"]] = subjectData[,phenoCol]
  res[["covariates"]] = as.matrix(subjectData[,firstCov:ncol(subjectData)])
  return (res)
  #  df <- read.table("c:/mplink/test/Gold/Original/testfam.fam", stringsAsFactors = FALSE)
  #  df2 <- df[,c(2:6)]
  #  x <- as.matrix(df[,1:2])
  #  return (x)
}

# @export
ReadFamilyFile <- function(familyFilename, useFamilyID = TRUE, usePhenotype = TRUE) {
  res <- list(success = FALSE, hasFamilyID = useFamilyID, subjects = NULL, phenotype = NULL)
  df <- read.table(familyFilename, stringsAsFactors = FALSE, header = FALSE)
  if (ncol(df) != 6) {
    print("Family file does not have 6 columns")
    return (res)
  }
  colnames(df) <- c("FID", "IID", "PID", "MID", "Sex", "Phenotype")
  if (usePhenotype) {
    if (is.numeric(df[,6]) == FALSE) {
      print("Column 6, phenotype is not numberic")
      return (res)
    }
    res$phenotype = as.vector(df[,6])
  }
  if (useFamilyID == FALSE)
    res$subjects <- as.matrix(df[,2])
  else
    res$subjects <- as.matrix(df[,1:2])
  
  res$success = TRUE
  return (res)
}
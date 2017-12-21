CreateFamilyFile <- function(filename) {
  df <- read.table(filename, stringsAsFactors = FALSE, header = FALSE)
  names(df) <- c("IID")
  df$FID <- 1:ncol(df)
  df$PID <- 0
  df$MID <- 0
  df$SEX <- 9
  df$STATUS <- 9
  df <- df[,c(2,1,3:6)]
  return (df)
}
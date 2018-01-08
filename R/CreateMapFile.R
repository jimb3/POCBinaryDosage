CreateMapFile <- function(SNPFile) {
  df <- read.table(SNPFile, header = TRUE, na.string = "-", stringsAsFactors = FALSE)
  df$CHR <- substr(df$SNP, 1, 2)
  df$cM <- 0
  df$BasePairs <- as.numeric(substr(df$SNP, 4, nchar(df$SNP)))
  df <- df[,c(15,1,16,17,2,3)]
  return (df)
}
allValues <- readr::read_table(file = "subset10.dosage", col_names = FALSE)
p0 <- allValues[,c(1,seq(5,248, by=3))]
p1 <- allValues[,c(1,seq(6,249, by=3))]
p2 <- allValues[,c(1,seq(7,250, by=3))]
subjects <- readr::read_table(file = "subset10.sample", col_names = FALSE)
subjects$so = as.numeric(1:nrow(subjects))
colnames(subjects) <- c("Subject", "Order")
columnNames <- "SNP"
columnNames <- rbind(columnNames, subjects[,1])
colnames(p0) <- columnNames$Subject
colnames(p1) <- columnNames$Subject
colnames(p2) <- columnNames$Subject
p0t <- tidyr::gather(p0, Subject, P0, -SNP)
p1t <- tidyr::gather(p1, Subject, P1, -SNP)
p2t <- tidyr::gather(p2, Subject, P2, -SNP)
p0p1 <- dplyr::inner_join(p0t, p1t)
dosage <- dplyr::inner_join(p0p1, p2t)
dosage <- dplyr::mutate(dosage, Dosage = P1 + P2 + P2)
dosage <- dplyr::inner_join(subjects, dosage)
locations <- allValues[,1:2]
colnames(locations) <- c("SNP", "BP")
allData <- dplyr::inner_join(locations, dosage)
allData <- dplyr::arrange(allData, BP, Order)
x <- as.matrix(allData[,5:8])

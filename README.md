# POCBinaryDosage
Testing of Binary Dosage files

This is a proof of concept for generating binary dosage files for GxEScan. This is not intended to be a complete package for creating binary dosage files.

This code uses a subset from a imputation run done using the HRC. The files have been converted from the VCF format that HRC normally uses and changed into a format similar to Impute 2 output. This is the format I received the files in and are simple to do test runs on.

The objective of this package is to create binary dosage files in all possible formats that can be then be used to verify GxEScan gets the same results regardless of the format used.

There are currently major formats used for the binary dosage files.

Format 1 stores the dosage data in short integers. These short integers are then divided by 32,767 to get the dosage values and 65,534 to get the genetic probabilities. Values of 65,535 indicate missing values.

Format 2 stores the dosage data in short integers. There short integers are then divided by 10,000 to get the dosage values and 20,000 to get the genetic probabilities. Values of 65,535 indicate missing. Values from 20,001 to 65,534 should not occur. This was since all dosage files I've seen have at most 4 significant digits passed the decimal point.

Format 3 is similar to format 2 but keeps enough values to fully recover all the values in the imputation file with no round off error. This occurs sometimes because P(g=1) + 2P(g=2) != dosage because of roundoff error in the impuation program. This format also has the number of subjects in the file stored. This simplifies verifying the file lines up with the list of subjects provided.

Format 4 is similar to format 3 but subject IDs and SNP information is stored in the binary dosage file. This eliminates the need for an external subject and SNP file. This is useful in preventing errors where the wrong subject or SNP file was used.

Each format has a subformat. Subformat 1 has only the dosage values in it. Subformat 2 has the genetic probabilities included.

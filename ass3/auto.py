import sys
import enchant
import csv

# The English library to validate words
d = enchant.Dict("en_US")

# The minimum length of the crib word
MIN_CRIB_LEN = 5

# The minimun length of the internal partial matching word
# This should be less or equal to MIN_CRIB_LEN
MIN_INTERNAL_LEN = 5 


# Load the 5,000 common English vocabulary library as crib word candidates
with open('FrequentEnglishWordList.csv', 'rb') as f:
    reader = csv.reader(f)
    crib_list = list(reader)
# Remove the header of the list file
crib_list = crib_list[1:]

# This function is credit to https://github.com/SpiderLabs/cribdrag
# @param: 	ctext	- 	The cipher text to be crib dragged
# @param: 	crib 	- 	The crib word to drag on the cipher text
# @return: 	results - 	A list of result that generated from a or partial match
#						of the crib word on the cipher text. Each result is
#						ensured to be an English word and contain letters only
def sxor(ctext, crib):
    results = []
    single_result = ''
    crib_len = len(crib)
    positions = len(ctext)-crib_len+1
    for index in xrange(positions):
        single_result = ''
        for a,b in zip(ctext[index:index+crib_len],crib):
            single_result += chr(ord(a) ^ ord(b))

        # Check the word partially, to see whether it conatins an English word
        result_length = len(single_result)
        if (result_length >= MIN_INTERNAL_LEN):
	        for i in xrange(0, result_length - MIN_INTERNAL_LEN):
	        	for j in xrange(i + MIN_INTERNAL_LEN, result_length):
	        		result_partial = single_result[i:j]
	        		if (result_partial.isalpha()):
	        			if (d.check(result_partial)):
	        				results.append(result_partial + "(" + str(index) + ")")

	    # Check the whole word, to see whether it is an English word
        if (single_result.isalpha()):
	    	    if(d.check(single_result)):
	    		      results.append(single_result + "(" + str(index) + ")")
	    
    return results

# This function executes the crib dragging of a single word on the cipher text
# And write/append the result to the target output file
def writeResultGivenCrib(ctext, crib):
	results = sxor(ctext, crib)
	results_len = len(results)
	# Write to the output (Append)
	with open(FILE_NAME, "a") as text_file:
		text_file.write(crib + "(" + str(results_len) + "): [ " + ', '.join(results) + " ]\n")

# Provide the Hexidecimal version of cipher_text here
CIPHER_HEX = "1a0e04061401041049171c57491308004f59041b01104e4105064c3a4f1b0045451a5f00154816570f045207114104531a1b45690013181b0c5e4527411801140a174e1913474e20010b1e5341060a000c450b1b4e131d01151c19451d5203040308140558491807541c480e024500455055595a554310540b0a191b4515551d0b050a49003d1c5218141448411a041f060016531c0115002d3613410c02251a030d11413901060a124311560a08540748044e3d4512024d4e121813010c49191d0e64310b0014420400451251105f494d030b16000c094e00010b552a16411a0845452d050214491c1d020d000408180d1a4e573a1b1d5409451f014e37090010460c1b1c0a144c3203000203490101001a62767f190b1054010753125943184f0032051216060e001e081b"

# Decode the hexidecimal cipher text
cipher_text = CIPHER_HEX.decode('hex')
# File Name
FILE_NAME = "auto_output.txt"

# Clear the content of the output file before starting to append
open(FILE_NAME, 'w').close()

# Iterate through the crib word list
for ind, crib in enumerate(crib_list):
	# Get the column of the word
	crib_word = crib[1]
	print str(ind) + ": " + crib_word
	# Write the result to the file
	if (len(crib_word) >= MIN_CRIB_LEN):
		writeResultGivenCrib(cipher_text, crib_word)

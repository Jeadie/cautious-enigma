import requests
import sys
import base64

URL = "http://ugster20.student.cs.uwaterloo.ca:4555"

def main():
    enc_cookie = sys.argv[1]
    enc_cookie = base64.b64decode(enc_cookie)
    blocks = blockise(enc_cookie)
    decrypt_blocks = decrypt(blocks)
    cookie = "".join(decrypt_blocks)
    print(base64.b64encode(cookie))


def blockise(data, block_size=16):
    block_list = []						 # list of blocks, we now want to break the cookie into blocks
    i = 0
    while i < len(data):				 
      	block_list.append(data[i:i+block_size])	# each block have length 16
      	i += 16



def decrypt_last_block(block):
    a_n = decrypt_last_world(block)



def decrypt_last_word(block)
    i = 0
    r = bytearray(16)

    code = 500
    while code==500:
          a = r[-1] = r[-1] ^ i
     			cur_cookie = {'user': base64.b64encode(a + block)}
    			r = requests.get(URL, cookies=cur_cookie)
          code = r.status_code          

     

def decrypt(data_blocks):
    result = []

    a_n = decrypt_last_block(data_blocks[-1])
    result.append(a_n)
    for i in range(len(data_blocks)-2, 0, -1):
        a_k =  decrypt_block(data_blocks[i], data_blocks[i+1])
        result.append(a_k)
    return result


    for i in range(len(block_list) - 1):
    	IV = block_list[i]   # the IV we use to get the plaintext , used later
    	cur_block = block_list[i+1]   # the current block, start with block 1
    	inter = bytearray(16)  # the intermidate value, all 0s in the beginning, we will fill in our guess value. It correspinds to the yellow values in the blog
    	middle_result = bytearray(16)  # the value we store to help update values in inter, it corresponds to the green values in the ref blog

    	for j in range(15, -1, -1): # now try from the last byte, which has index 15, unitl we guess to index 0
    		response_code = 500
    		while response_code != 200:  # we will keep guessing until we got the 200 response
    			inter[j] += 1
    			cur_cookie = {'user': base64.b64encode(inter + cur_block)} # pass curblock each time
    			r = requests.get('http://ugster20.student.cs.uwaterloo.ca:4555', cookies=cur_cookie)
    			response_code = r.status_code
    			# print response_code
    
    		# now the response code is 200, we have guessed the value
    		# now the inter will have i to the end bytes correctly guessed
    		middle_result[j] = inter[j] ^ (16-j)
    		inter[j] = 0 ^ middle_result[j]  # update to help guess the next byte, we do XOR 0 because of the padding format k0000000...
    	index = 0
    	temp = bytearray(len(middle_result))
    	while (index < len(middle_result)):
    		IV = bytearray(IV)
    		temp[index] = IV[index] ^ middle_result[index]
    		index += 1
    	result += temp


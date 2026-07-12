export LD_LIBRARY_PATH=./
#./simple_cpp_example --nthread=10 --loopcount=1000000
#./simple_cpp_example_direct --nthread=10 --loopcount=1000000
#./simple_c_example --nthread=10 --loopcount=1000000 --topic_index=5 --cfg=z.cfg
./simple_cpp_example --nthread=10 --loopcount=1000000 --mainindex=1 --topic_index=2 --topic_Bindex=1 --cfg=z.cfg


#Document:
#1. --cfg=		-->> Path to file of configuration. 	Mandatory
#2. --loopcount=	-->> Number of times for loop. 		Default: 1 million
#2. --nthread=		-->> Number od threads.			Default: 10
#4. --topic_index=	-->> 0: default. [1-n] for specific index, please see in file of configuration
#5. --topic_Bindex=	-->> 0: default. [1-m] for specific binary index, please see in file of configuration
#6. --mainindex=	-->> >= 0: enable of wrtting main topic 

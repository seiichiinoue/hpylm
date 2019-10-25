CC = clang++
LLDB = -g
BOOST = -lboost_serialization
PYTHON = -lboost_python37
GFLAGS = -lglog -lgflags
INCLUDE = -I/usr/local/lib `python3.7-config --include`
LDFLAGS = `python3.7-config --ldflags`

hpylm:
	$(CC) -O3 -o cstm src/model.cpp $(BOOST) $(GFLAGS)

install:
	$(CC) -O3 -DPIC -shared -fPIC -o pycstm.so pycstm.cpp $(INCLUDE) $(LDFLAGS) $(PYTHON) $(BOOST) $(GFLAGS)

test:
	$(CC) -O3 -Wall -o cstm src/model.cpp $(LLDB) $(BOOST) $(GFLAGS)

clean:
	rm -f cstm

.PHONY: clean
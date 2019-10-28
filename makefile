CC = clang++
LLDB = -g
BOOST = -lboost_serialization
PYTHON = -lboost_python37
INCLUDE = -I/usr/local/lib `python3.7-config --include`
LDFLAGS = `python3.7-config --ldflags`

hpylm:
	$(CC) -O3 -DPIC -shared -fPIC -o hpylm.so src/model.cpp $(INCLUDE) $(LDFLAGS) $(PYTHON) $(BOOST)

test:
	$(CC) -O3 -Wall -o hpylm src/model.cpp $(LLDB) $(INCLUDE) $(LDFLAGS) $(PYTHON) $(BOOST)

clean:
	rm -f cstm

.PHONY: clean
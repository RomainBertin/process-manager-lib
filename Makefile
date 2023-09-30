CXX = g++
CXXFLAGS = -Wall -std=c++11
OBJS = ProcessManager.o
BINDIR = ./bin
TARGET1 = $(BINDIR)/get_pid_by_name
TARGET2 = $(BINDIR)/kill_by_pid
TARGET3 = $(BINDIR)/kill_by_process_name

all: $(TARGET1) $(TARGET2) $(TARGET3)
	rm -f *.o

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET1): get-pid-by-name.o $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ get-pid-by-name.o $(OBJS)

$(TARGET2): kill-by-pid.o $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ kill-by-pid.o $(OBJS)

$(TARGET3): kill-by-process-name.o $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ kill-by-process-name.o $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)



SRCS = main.cpp Market.cpp TrendFollowingStrategy.cpp WeightedTrendFollowingStrategy.cpp \
       MeanReversionStrategy.cpp TradingBot.cpp Strategy.cpp Utils.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Uncomment the following line to enable sanitizers
# CXXFLAGS += -fsanitize=address,leak,undefined

ifeq ($(OS),Windows_NT)
	EXEC = pa2.exe
	RM = del
else
	EXEC = pa2
	RM = rm -f
endif

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

-include $(DEPS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -MMD -MP -c $<

clean:
	$(RM) $(EXEC) $(OBJS) $(DEPS)
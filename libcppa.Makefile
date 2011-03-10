CXX = /opt/local/bin/g++-mp-4.5
#CXXFLAGS = -std=c++0x -pedantic -Wall -Wextra -g -O0 -I/opt/local/include/
CXXFLAGS = -std=c++0x -pedantic -Wall -Wextra -O2 -I/opt/local/include/ -fPIC
LIBS = -L/opt/local/lib -lboost_thread-mt
INCLUDES = -I./

HEADERS = cppa/actor.hpp \
		  cppa/any_type.hpp \
		  cppa/config.hpp \
		  cppa/cow_ptr.hpp \
		  cppa/get.hpp \
		  cppa/intrusive_ptr.hpp \
		  cppa/invoke.hpp \
		  cppa/invoke_rules.hpp \
		  cppa/match.hpp \
		  cppa/message.hpp \
		  cppa/on.hpp \
		  cppa/ref_counted.hpp \
		  cppa/tuple.hpp \
		  cppa/tuple_view.hpp \
		  cppa/uniform_type_info.hpp \
		  cppa/untyped_tuple.hpp \
		  cppa/util.hpp \
		  cppa/detail/abstract_tuple.hpp \
		  cppa/detail/decorated_tuple.hpp \
		  cppa/detail/channel.hpp \
		  cppa/detail/intermediate.hpp \
		  cppa/detail/invokable.hpp \
		  cppa/detail/matcher.hpp \
		  cppa/detail/ref_counted_impl.hpp \
		  cppa/detail/scheduler.hpp \
		  cppa/detail/tdata.hpp \
		  cppa/detail/tuple_vals.hpp \
		  cppa/util/a_matches_b.hpp \
		  cppa/util/callable_trait.hpp \
		  cppa/util/compare_tuples.hpp \
		  cppa/util/concat_type_lists.hpp \
		  cppa/util/conjunction.hpp \
		  cppa/util/detach.hpp \
		  cppa/util/disjunction.hpp \
		  cppa/util/eval_type_lists.hpp \
		  cppa/util/filter_type_list.hpp \
		  cppa/util/has_copy_member_fun.hpp \
		  cppa/util/is_comparable.hpp \
		  cppa/util/is_copyable.hpp \
		  cppa/util/is_one_of.hpp \
		  cppa/util/remove_const_reference.hpp \
		  cppa/util/reverse_type_list.hpp \
		  cppa/util/single_reader_queue.hpp \
		  cppa/util/type_at.hpp \
		  cppa/util/type_list.hpp \
		  cppa/util/type_list_apply.hpp \
		  cppa/util/type_list_pop_back.hpp \
		  cppa/util/utype_iterator.hpp \
		  cppa/util/void_type.hpp

SOURCES = src/actor.cpp \
		  src/decorated_tuple.cpp \
		  src/deserializer.cpp \
		  src/message_receiver.cpp \
		  src/mock_scheduler.cpp \
		  src/ref_counted.cpp \
		  src/serializer.cpp \
		  src/uniform_type_info.cpp \
		  src/untyped_tuple.cpp

OBJECTS = $(SOURCES:.cpp=.o)

LIB_NAME = libcppa.dylib

%.o : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fPIC -c $< -o $@

$(LIB_NAME) : $(OBJECTS) $(HEADERS)
	$(CXX) $(LIBS) -dynamiclib -o $(LIB_NAME) $(OBJECTS)

all : $(LIB_NAME) $(OBJECTS)

clean:
	rm -f $(LIB_NAME) $(OBJECTS)


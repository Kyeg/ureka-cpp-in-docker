CXX = g++
CXXFLAGS = -lmbedtls -lmbedcrypto -lmbedx509 -lgtest -lgtest_main # -lpthread 
SRC = ../logic/pipeline_flow/flow_issue_u_ticket.cpp ../environment.cpp ../logic/pipeline_flow/flow_apply_u_ticket.cpp ../logic/pipeline_flow/flow_open_session.cpp ../logic/pipeline_flow/flow_issue_u_token.cpp
SRC1 = ../logic/stage_worker/msg_generator.cpp ../logic/stage_worker/msg_verifier.cpp ../logic/stage_worker/msg_verifier_message.cpp ../logic/stage_worker/msg_verifier_r_ticket.cpp ../logic/stage_worker/msg_verifier_u_ticket.cpp ../logic/stage_worker/msg_generator_r_ticket.cpp ../logic/stage_worker/msg_generator_u_ticket.cpp ../logic/stage_worker/executor.cpp ../logic/stage_worker/generated_msg_storer.cpp ../logic/stage_worker/msg_sender.cpp ../logic/stage_worker/msg_receiver.cpp ../logic/stage_worker/received_msg_storer.cpp  ../logic/device_controller.cpp 
SRC2 = ../resource/crypto/ecc.cpp ../resource/crypto/ecdh.cpp ../model/message_model/u_ticket.cpp ../model/message_model/r_ticket.cpp ../model/message_model/message.cpp ../model/data_model/other_device.cpp ../model/data_model/this_person.cpp ../model/data_model/this_device.cpp ../model/data_model/current_session.cpp
OBJ = $(SRC:.cpp=.o) $(SRC2:.cpp=.o) $(SRC1:.cpp=.o)
TARGET1 = my_program
TARGET2 = my_program2
TARGET3 = my_program3
TARGET4 = my_program4
TARGET5 = my_program5
TARGET6 = my_program6
TARGET7 = my_program7
TARGET8 = my_program8
TARGET9 = my_program9
TARGET10 = my_program10
TARGET11 = my_program11
TARGET12 = my_program12
TARGET13 = my_program13
TARGET14 = my_program14
TARGET15 = my_program15
TARGET16 = my_program16
TARGET17 = my_program17
TARGET18 = my_program18

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET16) $(TARGET17) $(TARGET18)

$(TARGET1): $(OBJ)
	$(CXX) operation/test_01.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET1) 

$(TARGET2): $(OBJ)
	$(CXX) operation/test_02.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET2)

$(TARGET3): $(OBJ)
	$(CXX) operation/test_03.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET3)

$(TARGET4): $(OBJ)
	$(CXX) operation/test_04.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET4)

$(TARGET5): $(OBJ)
	$(CXX) operation/test_05.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET5)

$(TARGET6): $(OBJ)
	$(CXX) operation/test_06.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET6)

$(TARGET7): $(OBJ)
	$(CXX) operation/test_07.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET7)

$(TARGET8): $(OBJ)
	$(CXX) operation/test_08.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET8)

$(TARGET9): $(OBJ)
	$(CXX) operation/test_09.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET9)

$(TARGET10): $(OBJ)
	$(CXX) operation/test_14_1.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET10)

$(TARGET11): $(OBJ)
	$(CXX) operation/test_14_2.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET11)

$(TARGET12): $(OBJ)
	$(CXX) operation/test_14_3.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET12)

$(TARGET13): $(OBJ)
	$(CXX) operation/test_15_1.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET13)

$(TARGET14): $(OBJ)
	$(CXX) operation/test_15_2.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET14)

$(TARGET15): $(OBJ)
	$(CXX) operation/test_15_3.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET15)

$(TARGET16): $(OBJ)
	$(CXX) operation/test_16_1.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET16)

$(TARGET18): $(OBJ)
	$(CXX) operation/test_16_3.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET18)

program: $(TARGET18)

$(TARGET17): $(OBJ)
	$(CXX) operation/test_16_2.cpp $(OBJ) $(CXXFLAGS)  -o $(TARGET17)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean_all:
	rm -f $(OBJ) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET16) $(TARGET17) $(TARGET18)

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET16) $(TARGET17) $(TARGET18)

run:
	./$(TARGET1)
	./$(TARGET2)
	./$(TARGET3)
	./$(TARGET4)
	./$(TARGET5)
	./$(TARGET6)
	./$(TARGET7)
	./$(TARGET8)
	./$(TARGET9)
	./$(TARGET10)
	./$(TARGET11)
	./$(TARGET12)
	./$(TARGET13)
	./$(TARGET14)
	./$(TARGET15)
	./$(TARGET16)
	./$(TARGET17)
	./$(TARGET18)
	



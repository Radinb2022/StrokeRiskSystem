.PHONY: all clean test run

all: compile

compile:
	mvn compile

test:
	mvn test

run:
	mvn exec:java -Dexec.mainClass="src.Main"

clean:
	mvn clean

#!/bin/bash

source build.sh

# Define the common input files
VECTOR_TABLE="vector_table.txt"
DEVICE_TABLE="device_table.txt"

# Baseline Tests (Cases 1-20)
echo "Running baseline test cases 1-20..."
for i in {1..20}
do
  TRACE_FILE="testcases/trace_${i}.txt"
  OUTPUT_FILE="executions/execution_${i}.txt"
  if [ -f "$TRACE_FILE" ]; then
    # Use default values: context_time=10, isr_time=40
    ./bin/interrupts "$TRACE_FILE" "$VECTOR_TABLE" "$DEVICE_TABLE" "$OUTPUT_FILE" 10 40
  else
    echo "Warning: Trace file ${TRACE_FILE} not found. Skipping."
  fi
done

#Parametric Sweep (Cases 21-35)
echo "Running parametric sweep tests 21-35..."
TRACE_FILE="testcases/trace_1.txt"
EXEC_NUM=21

# Outer loop for context_time
for CONTEXT_TIME in 10 20 30
do
  # Inner loop for isr_time
  ISR_TIME=40
  for j in {1..5}
  do
    OUTPUT_FILE="executions/execution_${EXEC_NUM}.txt"
    echo "Running case ${EXEC_NUM}: context_time=${CONTEXT_TIME}, isr_time=${ISR_TIME}"
    ./bin/interrupts "$TRACE_FILE" "$VECTOR_TABLE" "$DEVICE_TABLE" "$OUTPUT_FILE" $CONTEXT_TIME $ISR_TIME
    
    # Increment for next run
    ISR_TIME=$((ISR_TIME + 40))
    EXEC_NUM=$((EXEC_NUM + 1))
  done
done

echo "All 35 test cases have been processed."

import serial
import time

# Initialize the serial port (adjust COM port and baud rate to match your setup)
ser = serial.Serial('COM4', 9600, timeout=1)

def get_distance():
    # Send 0x55 command to start measurement
    ser.write(b'\x55')  # Trigger measurement command
    
    # Wait for the sensor to process the request (adjust delay as needed)
    time.sleep(.7)  # 500ms delay to allow sensor time to measure
    
    # Read 2 bytes of data from the sensor (16-bit distance value)
    rx_buffer = ser.read(2)

    # Check if 2 bytes are received
    if len(rx_buffer) == 2:
        # Debugging: Print the raw received data
        print(f"Received raw data: {rx_buffer.hex()}")

        # Check for valid sync bytes (optional)
        if rx_buffer[1] == 0xFF and rx_buffer[0] == 0xFF:
            print("Sync bytes received correctly.")
        
        # Combine the two bytes into a 16-bit distance value
        distance = (rx_buffer[1] << 8) | rx_buffer[0]  # MSB at index 1, LSB at index 0
        print(f"Distance (raw): {distance} mm")
        
        return distance
    else:
        print("Error: Invalid data frame received")
    return None


def main():
    while True:
        distance = get_distance()
        if distance is not None:
            print(f"Distance: {distance} mm")
        else:
            print("Error: Failed to get distance")
        
        time.sleep(1)  # Delay 1 second before the next measurement

if __name__ == '__main__':
    main()

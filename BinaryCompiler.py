###################################
# PROPERTY OF THE EYESCIRE ENGINE #
###################################

import struct

def convert_object_to_binary(dimension ,object_name, object_info, texure, x, y, z):
    """
    Converts object information into a custom binary format and returns
    the binary data and its hexadecimal string representation.
    """
    # Create the binary data using Python's struct module.
    # The format string uses little-endian byte order '<'
    # and specifies the data types:
    # 'I' for unsigned 32-bit integer
    # 'f' for 32-bit float
    
    # 1. Name Length (unsigned 32-bit int) and Name String (bytes)
    name_bytes = object_name.encode('utf-8')
    name_length = len(name_bytes)
    
    # 2. Info Length (unsigned 32-bit int) and Info String (bytes)
    info_bytes = object_info.encode('utf-8')
    info_length = len(info_bytes)
    
    # 3. Position (x, y, z as 32-bit floats)
    
    # Pack the data in the specified order
    binary_data = struct.pack('<I', name_length) + \
                  name_bytes + \
                  struct.pack('<I', info_length) + \
                  info_bytes + \
                  struct.pack('<fff', x, y, z)
    
    # Convert the binary data to its hexadecimal representation
    hex_string = binary_data.hex()
    
    return binary_data, hex_string

def write_binary_file(filename, binary_data):
    """Writes binary data to a file."""
    with open(filename, 'wb') as f:
        f.write(binary_data)
        
if __name__ == '__main__':
    # --- Data for a single object ---
    dimension =  "3D"
    name = "Wall"
    info = "Mesh-Cylinder"
    texure = "blah.png"
    pos_x = 0.5
    pos_y = 10.0
    pos_z = 8.0
    
    # --- Convert and get the data ---
    binary_data, hex_string = convert_object_to_binary(dimension, name, info, texure, pos_x, pos_y, pos_z)
    
    # --- Print the results ---
    print(f"Object Name:   {name}")
    print(f"Object Info:   {info}")
    print(f"Position:      ({pos_x}, {pos_y}, {pos_z})")
    print("\n--- Binary Output ---")
    print(f"Hex String:    {hex_string}")
    print(f"Raw Bytes:     {binary_data}")
    
    # --- Write to a file ---
    filename = "test_object.eys"
    write_binary_file(filename, binary_data)
    print(f"\nSuccessfully wrote binary data to {filename}")


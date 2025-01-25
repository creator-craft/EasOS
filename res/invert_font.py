def inv_byte(b):
  return ((b & 1) << 7) + ((b & 2) << 5) + ((b & 4) << 3) + ((b & 8) << 1) + ((b & 16) >> 1) + ((b & 32) >> 3) + ((b & 64) >> 5) + (b >> 7)

with open("res/VGA-8x16.font", "rb") as infile, open("res/VGA-8x16_.font", "wb") as outfile:
    outfile.write(bytes(inv_byte(b) for b in infile.read()))
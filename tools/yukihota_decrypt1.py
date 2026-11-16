import os
import sys


def decrypt_file(file_path: str):
  """Decrypts a file using the byte-negation algorithm: lpBuffer[i] = -lpBuffer[i]."""
  if not os.path.exists(file_path):
    print(f"[Error] File not found: {file_path}")
    return

  # Split filename and extension
  directory, filename = os.path.split(file_path)
  name, ext = os.path.splitext(filename)

  # Construct output path: filename + "_decrypted" + extension
  output_filename = f"{name}_decrypted{ext}"
  output_path = (
      os.path.join(directory, output_filename) if directory else output_filename
  )

  try:
    # Read the encrypted file as raw bytes
    with open(file_path, "rb") as f:
      encrypted_data = f.read()

    # Apply the C equivalent: lpBuffer[i] = -lpBuffer[i]
    # In Python, byte values range from 0 to 255. Negating and masking with & 0xFF
    # ensures it wraps around correctly within the single-byte range.
    decrypted_bytes = bytes((-b) & 0xFF for b in encrypted_data)

    # Write the decrypted data to the new file
    with open(output_path, "wb") as f:
      f.write(decrypted_bytes)

    print(f"[Success] Decrypted file saved as: {output_path}")

  except Exception as e:
    print(f"[Error] An error occurred during decryption: {e}")


if __name__ == "__main__":
  if len(sys.argv) < 2:
    # You can also hardcode a filename here if you prefer not to use CLI args
    target_file = input("Enter path to the file to decrypt: ").strip('"')
  else:
    target_file = sys.argv[1]

  decrypt_file(target_file)
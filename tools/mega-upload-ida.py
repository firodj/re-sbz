import asyncio
import os
from mega.client import MegaNzClient  # async-mega-py uses the same import name but functions are async
from dotenv import load_dotenv
load_dotenv()

appdir = os.getenv('APPDIR')
p7zpath = os.getenv('7ZPATH')

# --- CONFIGURATION ---
FILE_TO_COMPRESS = os.path.join(appdir, "Sexy Beach Zero English.exe.i64")
OUTPUT_7Z_FILE = "Sexy Beach Zero English (IDA 9.1).7z"

MEGA_EMAIL = os.getenv('MEGAUSER')
MEGA_PASSWORD = os.getenv('MEGAPASS')
# ---------------------

async def compress_to_7z_async(input_file, output_file):
    """Asynchronously compresses a file using the 7z command line tool."""
    if os.path.exists(output_file):
        print(f"Output file '{output_file}' already exists. Skipping compression.")
        return True
        
    print(f"Compressing '{input_file}' into '{output_file}' asynchronously...")
    
    try:
        # Create an async subprocess
        process = await asyncio.create_subprocess_exec(
            os.path.join(p7zpath, "7za.exe"), "a", output_file, input_file,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE
        )
        
        # Wait for the compression to finish
        stdout, stderr = await process.communicate()
        
        if process.returncode == 0:
            print("Compression successful!")
            return True
        else:
            print(f"Compression failed with exit code {process.returncode}")
            print(f"Error details: {stderr.decode().strip()}")
            return False
            
    except FileNotFoundError:
        print("Error: '7z' command not found. Please ensure 7-Zip is installed and in your PATH.")
        return False

async def upload_to_mega_async(file_path, email, password):
    """Asynchronously logs into MEGA and uploads the file."""
    print(f"Connecting to MEGA and uploading '{file_path}'...")
    try:
        # Initialize async MEGA
        mega = MegaNzClient()
        
        # Log in asynchronously
        await mega.login(email, password)

        user = await mega.get_user()
        print("Logged in successfully")
        print("User: ", user.get('email'))

        # Find Research Folder
        fs = await mega.get_filesystem()

        # Search for nodes
        query = "/Research"
        research_node_id = None
        for node_id, path in fs.search(query):
            #print(node_id, path)
            if str(path) == query:
                research_node_id = node_id
                break
        print(f"Found Research Node: {research_node_id}")
        
        # Upload file asynchronously
        uploaded_file = await mega.upload(file_path, research_node_id)
        print("Upload successful!")
        await mega.close()
        return True
        
    except Exception as e:
        print(f"An error occurred during MEGA upload: {e}")
        return False

async def main():
    # Step 1: Compress the file asynchronously
    compression_success = await compress_to_7z_async(FILE_TO_COMPRESS, OUTPUT_7Z_FILE)
    
    if compression_success:
        # Step 2: Upload asynchronously
        await upload_to_mega_async(OUTPUT_7Z_FILE, MEGA_EMAIL, MEGA_PASSWORD)
        
        # Optional Step 3: Clean up local file
        # if os.path.exists(OUTPUT_7Z_FILE):
        #     os.remove(OUTPUT_7Z_FILE)

if __name__ == "__main__":
    if os.path.exists(FILE_TO_COMPRESS):
        # Start the asyncio event loop
        asyncio.run(main())
    else:
        print(f"Error: The file '{FILE_TO_COMPRESS}' does not exist.")
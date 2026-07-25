import asyncio
import os, sys
from mega.client import MegaNzClient  # async-mega-py uses the same import name but functions are async
from dotenv import load_dotenv
load_dotenv()

# --- CONFIGURATION ---
p7zpath = os.getenv('7ZPATH')
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

async def run(idafile, out7zfile):
    # Step 1: Compress the file asynchronously
    compression_success = await compress_to_7z_async(idafile, out7zfile)
    
    if compression_success:
        # Step 2: Upload asynchronously
        await upload_to_mega_async(out7zfile, MEGA_EMAIL, MEGA_PASSWORD)
        
        # Optional Step 3: Clean up local file
        if os.path.exists(out7zfile):
            os.remove(out7zfile)

def main() -> int:
    argc = len(sys.argv)

    if argc > 1:
        first_arg = sys.argv[1]
    else:
        print("please provide the app-id")
        return 1
    
    match first_arg:
        case "sbz":
            appdir = os.getenv('SBZ_APPDIR')
            idafile = os.path.join(appdir, "Sexy Beach Zero English.exe.i64")
            out7zfile = "Sexy Beach Zero English (IDA 9.1).7z"
        case "hnh":
            appdir = os.getenv('HNH_APPDIR')
            idafile = os.path.join(appdir, "HaNaHiMe.exe.i64")
            out7zfile = "HaNaHiMe (IDA 9.1).7z"
        case _:
            print(f"unknown {first_arg}, allowed: sbz, hnh")
            return 1
    
    if os.path.exists(idafile):
        # Start the asyncio event loop
        asyncio.run(run(idafile, out7zfile))
    else:
        print(f"Error: The file '{idafile}' does not exist.")

    return 0

if __name__ == "__main__":
    exit_code = main()
    sys.exit(exit_code)
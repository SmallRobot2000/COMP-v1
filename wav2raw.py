import wave

def extract_samples_to_raw(wav_file_path, raw_file_path):
    try:
        # Open the .wav file
        with wave.open(wav_file_path, 'rb') as wav_file:
            # Ensure it is 8-bit audio
            if wav_file.getsampwidth() != 1:
                raise ValueError("This script only works with 8-bit WAV files.")
            
            # Get the number of frames (samples)
            n_frames = wav_file.getnframes()
            
            # Read all frames
            raw_data = wav_file.readframes(n_frames)
            
            # Convert unsigned 8-bit samples to signed 8-bit samples
            signed_samples = bytearray([sample - 128 for sample in raw_data])
            
        # Write the signed samples to a .raw file
        with open(raw_file_path, 'wb') as raw_file:
            raw_file.write(signed_samples)
            
        print(f"Signed 8-bit samples written to {raw_file_path} successfully!")
    
    except wave.Error as e:
        print(f"Error reading WAV file: {e}")
    except ValueError as e:
        print(e)

# Example usage
wav_file_path = 'very bad apple.wav'  # Replace with your .wav file path
raw_file_path = 'output.raw'   # Replace with desired .raw file path

extract_samples_to_raw(wav_file_path, raw_file_path)


import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox, filedialog
import subprocess
import os
import sys
import traceback

class CompilerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Simple C Compiler")
        self.root.geometry("1000x800")
        
        # Main frame
        self.main_frame = ttk.Frame(root, padding="10")
        self.main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        root.grid_rowconfigure(0, weight=1)
        root.grid_columnconfigure(0, weight=1)

        # Title
        title_label = ttk.Label(self.main_frame, 
                               text="C Code Analyzer", 
                               font=('Helvetica', 16, 'bold'))
        title_label.grid(row=0, column=0, columnspan=2, pady=(0, 10))

        # Code Editor
        editor_frame = ttk.LabelFrame(self.main_frame, text="Source Code", padding="5")
        editor_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.main_frame.grid_rowconfigure(1, weight=1)
        
        self.code_input = scrolledtext.ScrolledText(
            editor_frame, 
            width=80, 
            height=15,
            font=('Consolas', 11)
        )
        self.code_input.pack(expand=True, fill='both', padx=5, pady=5)

        # File Buttons Frame
        file_button_frame = ttk.Frame(self.main_frame)
        file_button_frame.grid(row=2, column=0, columnspan=2, pady=5)
        
        ttk.Button(file_button_frame, text="Open File", command=self.open_file).pack(side=tk.LEFT, padx=5)
        ttk.Button(file_button_frame, text="Save File", command=self.save_file).pack(side=tk.LEFT, padx=5)
        ttk.Button(file_button_frame, text="Clear", command=self.clear_all).pack(side=tk.LEFT, padx=5)

        # Compilation Steps Frame
        steps_frame = ttk.LabelFrame(self.main_frame, text="Compilation Steps", padding="5")
        steps_frame.grid(row=3, column=0, columnspan=2, pady=5, sticky=(tk.W, tk.E))
        
        # Style for step buttons
        style = ttk.Style()
        style.configure("Step.TButton", padding=5)
        
        # Step buttons
        ttk.Button(steps_frame, text="1. Lexical Analysis", 
                  command=self.run_lexical_analysis, style="Step.TButton").pack(side=tk.LEFT, padx=5, pady=5)
        ttk.Button(steps_frame, text="2. Parse Tree", 
                  command=self.run_parse_tree, style="Step.TButton").pack(side=tk.LEFT, padx=5, pady=5)
        ttk.Button(steps_frame, text="3. Compile & Run", 
                  command=self.compile_and_run, style="Step.TButton").pack(side=tk.LEFT, padx=5, pady=5)

        # Output Area
        output_frame = ttk.LabelFrame(self.main_frame, text="Analysis Output", padding="5")
        output_frame.grid(row=4, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.main_frame.grid_rowconfigure(4, weight=1)

        self.output_text = scrolledtext.ScrolledText(
            output_frame,
            height=15,
            font=('Consolas', 11)
        )
        self.output_text.pack(expand=True, fill='both', padx=5, pady=5)

        # Add sample code
        self.add_sample_code()

    def add_sample_code(self):
        sample_code = '''/* Enter your C code here */
#include <stdio.h>

int main() {
    printf("Hello, World!\\n");
    return 0;
}'''
        self.code_input.insert('1.0', sample_code)

    def open_file(self):
        file_path = filedialog.askopenfilename(
            filetypes=[("C files", "*.c"), ("Header files", "*.h"), ("All files", "*.*")]
        )
        if file_path:
            try:
                with open(file_path, 'r') as file:
                    self.code_input.delete(1.0, tk.END)
                    self.code_input.insert(1.0, file.read())
            except Exception as e:
                messagebox.showerror("Error", f"Could not open file: {str(e)}")

    def save_file(self):
        file_path = filedialog.asksaveasfilename(
            defaultextension=".c",
            filetypes=[("C files", "*.c"), ("Header files", "*.h"), ("All files", "*.*")]
        )
        if file_path:
            try:
                with open(file_path, 'w') as file:
                    file.write(self.code_input.get(1.0, tk.END))
            except Exception as e:
                messagebox.showerror("Error", f"Could not save file: {str(e)}")

    def clear_all(self):
        self.code_input.delete(1.0, tk.END)
        self.output_text.delete(1.0, tk.END)

    def run_lexical_analysis(self):
        try:
            self.output_text.delete(1.0, tk.END)
            self.output_text.insert(tk.END, "Running Lexical Analysis...\n\n")
            
            # Save code to temporary file
            temp_file = "temp_code.c"
            with open(temp_file, 'w') as f:
                f.write(self.code_input.get(1.0, tk.END))

            # Run parser with lexical analysis flag
            parser_path = os.path.join(os.getcwd(), "parser.exe")
            result = subprocess.run([parser_path, temp_file, "--lexical"], 
                                 capture_output=True, 
                                 text=True)
            
            if result.stdout:
                # Only display token information
                self.output_text.insert(tk.END, result.stdout)
            if result.stderr:
                self.output_text.insert(tk.END, "\nErrors:\n" + result.stderr)

            # Clean up
            if os.path.exists(temp_file):
                os.remove(temp_file)

        except Exception as e:
            self.output_text.insert(tk.END, f"Error: {str(e)}\n{traceback.format_exc()}")

    def run_parse_tree(self):
        try:
            self.output_text.delete(1.0, tk.END)
            self.output_text.insert(tk.END, "Generating Parse Tree...\n\n")
            
            temp_file = "temp_code.c"
            with open(temp_file, 'w') as f:
                f.write(self.code_input.get(1.0, tk.END))

            parser_path = os.path.join(os.getcwd(), "parser.exe")
            result = subprocess.run([parser_path, temp_file, "--parse-tree"], 
                                 capture_output=True, 
                                 text=True)
            
            if result.stdout:
                # Only display parse tree information
                self.output_text.insert(tk.END, result.stdout)
            if result.stderr:
                self.output_text.insert(tk.END, "\nErrors:\n" + result.stderr)

            # Clean up
            if os.path.exists(temp_file):
                os.remove(temp_file)

        except Exception as e:
            self.output_text.insert(tk.END, f"Error: {str(e)}\n{traceback.format_exc()}")

    def compile_and_run(self):
        try:
            self.output_text.delete(1.0, tk.END)
            self.output_text.insert(tk.END, "Compiling and Running...\n\n")
            
            # Save to temporary file
            temp_file = "temp_code.c"
            with open(temp_file, 'w') as f:
                f.write(self.code_input.get(1.0, tk.END))

            # Compile
            compile_result = subprocess.run(["gcc", temp_file, "-o", "program"], 
                                         capture_output=True, 
                                         text=True)

            if compile_result.returncode != 0:
                self.output_text.insert(tk.END, "Compilation Error:\n")
                self.output_text.insert(tk.END, compile_result.stderr)
                return

            # Run
            try:
                run_result = subprocess.run(["./program"], 
                                         capture_output=True,
                                         text=True,
                                         timeout=5)
                
                self.output_text.insert(tk.END, "Program Output:\n")
                if run_result.stdout:
                    self.output_text.insert(tk.END, run_result.stdout)
                if run_result.stderr:
                    self.output_text.insert(tk.END, "\nErrors:\n" + run_result.stderr)
                    
            except subprocess.TimeoutExpired:
                self.output_text.insert(tk.END, "\nProgram timed out after 5 seconds")

            # Cleanup
            if os.path.exists(temp_file):
                os.remove(temp_file)
            if os.path.exists("program"):
                os.remove("program")
            if os.path.exists("program.exe"):
                os.remove("program.exe")

        except Exception as e:
            self.output_text.insert(tk.END, f"Error: {str(e)}\n{traceback.format_exc()}")

def main():
    root = tk.Tk()
    app = CompilerGUI(root)
    root.mainloop()

if __name__ == "__main__":
    main() 
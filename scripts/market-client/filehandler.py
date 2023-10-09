class FileHandler:
    
    def __init__(self, file_path):
        self.file_path = file_path
        self.file = None

    def create_file(self):
        try:
            with open(self.file_path, 'x'):
                pass
        except FileExistsError:
            pass

    def open_file(self):
        self.file = open(self.file_path, 'w+')

    def write_to_file(self, data):
        if self.file:
            self.file.write(data)
            self.file.flush()

    def read_file(self):
        if self.file:
            self.file.seek(0)
            content = self.file.read()
            return content
        else:
            return None
    
    def truncate(self):
        if self.file:
            self.file.truncate()

    def close_file(self):
        if self.file:
            self.file.close()


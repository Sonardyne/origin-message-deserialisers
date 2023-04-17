from abc import ABC, abstractmethod

class RDIPD0Interface(ABC):
    def __init__(self):
        super().__init__()
    
    @abstractmethod
    def get_id(self):
        pass
    
    @abstractmethod
    def calc_length(self):
        pass
    
    @abstractmethod
    def serialise(self, msg_bytes):
        pass
    
    @abstractmethod
    def deserialise(self, msg_bytes):
        pass

    @abstractmethod
    def show(self):
        pass


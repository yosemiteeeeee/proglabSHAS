#include <iostream>
#include <windows.h>
#include <vector>
#pragma pack(push,1)
using namespace std;

// struct Volume Header
struct VolumeHeader
{
    WORD header_crc;
    BYTE header_type;
    WORD header_flags;
    WORD header_size;
};

// struct File_head
struct Filehead
{
    DWORD pack_size;
    DWORD unpsize;
    BYTE HostOS;
    DWORD fileCRC;
    DWORD FileTime;
    BYTE unpver;
    BYTE method;
    WORD namesize;
    DWORD fileattr;
};

#pragma pack(pop)

// Signatures
const BYTE signatures[] = {0x00, 0x07, 0x21, 0x52, 0x61, 0x72, 0x1A};

// Prototype of function for outputting file names
void PrintHeaderContent(const vector <char> &, DWORD filesize);

void Print_vector(const vector <char> & printing_vector, int vectorsize);

int main() {
    // Open file
    HANDLE file_to_open_Handler = CreateFile("Example.rar", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_to_open_Handler == INVALID_HANDLE_VALUE) {
        cout << "Error opening file." << endl;
        return 1;
    }
    // Get file size
    DWORD fileSize = GetFileSize(file_to_open_Handler, NULL);

    // Transfer file contents to buffer
    vector<char> buffer(fileSize);
    DWORD bytesRead;
    ReadFile(file_to_open_Handler, &buffer[1], fileSize, &bytesRead, NULL);
    PrintHeaderContent(buffer, fileSize);
    // Close file
    CloseHandle(file_to_open_Handler);
    return 0;
}
void Print_vector(const vector <char> & printing_vector) {
    for (char p : printing_vector){
        cout << p;
    }
}
void DebugVar(auto variable){
    cout << "\n!!!!!" << variable << endl;
}
void PrintHeaderContent(const vector <char> & filebufer, DWORD filesize) {
    // Create Volume Header example
    cout << "\nFile size: " <<  filesize << " bytes" << endl ;
    VolumeHeader header_main;
    // Create header file example
    Filehead file_header;
    int index_to_cut = sizeof(signatures); // Skip first 7 bytes
    int count_of_files = 0;
    while (index_to_cut < filesize) {
        memcpy(&header_main, &filebufer[index_to_cut], sizeof(header_main));
        if (header_main.header_type == 0x74) {
            cout << "\n    !!Byte 0x74!!    " << endl;
            int temporary = index_to_cut;
            count_of_files++ ;
            cout << "File " << count_of_files << ": " << endl;
            index_to_cut += sizeof(header_main) ;
            memcpy(&file_header, &filebufer[index_to_cut], sizeof(file_header));
            index_to_cut += sizeof(file_header);
            // Char massive with file name
            char *FileName = new char[file_header.namesize];
            for (int index_to_file = 0 ; index_to_file < file_header.namesize; index_to_file++) {
                FileName[index_to_file] = filebufer[index_to_cut];
                index_to_cut++;
                cout << FileName[index_to_file];
            }
            cout << "\n";
            index_to_cut = temporary;
            index_to_cut += file_header.pack_size + header_main.header_size;
            delete[] FileName;
        } else {
            // Shift by header size
            index_to_cut += sizeof(header_main.header_size);
        }
    }
    wcout << "\nNumber of files in the archive: " << count_of_files;
}

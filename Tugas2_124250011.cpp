#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
using namespace std;

struct Karyawan {
    int nip;
    string nama;
    string divisi;
    string status;
};

struct Node {
    Karyawan data;
    Node* left;
    Node* right;
    Node(Karyawan k) {
        data = k;
        left = right = NULL;
    }
};

Node* root = NULL;
stack<pair<int,string>> riwayat;
queue<int> antrian;

// ===== BST =====
Node* insert(Node* node, Karyawan k) {
    if (node == NULL) return new Node(k);
    if (k.nip < node->data.nip) node->left = insert(node->left, k);
    else if (k.nip > node->data.nip) node->right = insert(node->right, k);
    return node;
}

Node* cari(Node* node, int nip) {
    if (node == NULL) return NULL;
    if (nip == node->data.nip) return node;
    if (nip < node->data.nip) return cari(node->left, nip);
    else return cari(node->right, nip);
}

Node* hapus(Node* node, int nip) {
    if (node == NULL) return NULL;
    if (nip < node->data.nip) node->left = hapus(node->left, nip);
    else if (nip > node->data.nip) node->right = hapus(node->right, nip);
    else {
        if (node->left == NULL) return node->right;
        else if (node->right == NULL) return node->left;
    }
    return node;
}

void garisTabel() {
    cout << "+----------+---------------+---------------+---------------+" << endl;
}

void tampil(Node* node) {
    if (node) {
        tampil(node->left);
        cout << "| " << setw(8) << node->data.nip
             << " | " << setw(13) << node->data.nama
             << " | " << setw(13) << node->data.divisi
             << " | " << setw(13) << node->data.status << " |" << endl;
        garisTabel();
        tampil(node->right);
    }
}

void tampilKaryawan() {
    cout << "\nDaftar Karyawan:\n";
    garisTabel();
    cout << "| " << setw(8) << "NIP"
         << " | " << setw(13) << "Nama"
         << " | " << setw(13) << "Divisi"
         << " | " << setw(13) << "Status" << " |" << endl;
    garisTabel();
    tampil(root);
}

void tambahKaryawan(int nip, string nama, string divisi) {
    Karyawan k = {nip, nama, divisi, "Tidak Cuti"};
    root = insert(root, k);
    cout << "Data karyawan berhasil ditambahkan\n";
}

void editKaryawan(int nip, string namaBaru, string divisiBaru) {
    Node* n = cari(root, nip);
    if (n == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }
    n->data.nama = namaBaru;
    n->data.divisi = divisiBaru;
    cout << "Data karyawan berhasil diedit\n";
}

void ajukanCuti(int nip) {
    Node* n = cari(root, nip);
    if (n == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }
    if (n->data.status == "Tidak Cuti") {
        n->data.status = "Cuti";
        riwayat.push({nip,"Pengajuan"});
        cout << "Pengajuan cuti berhasil\n";
    } else {
        antrian.push(nip);
        riwayat.push({nip,"Antrian"});
        cout << "Karyawan sedang cuti, pengajuan masuk ke dalam antrian\n";
    }
}

void selesaiCuti(int nip) {
    Node* n = cari(root, nip);
    if (n == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }
    if (n->data.status == "Cuti") {
        n->data.status = "Tidak Cuti";
        riwayat.push({nip,"Selesai"});
        cout << "Cuti berhasil diselesaikan\n";
        if (!antrian.empty() && antrian.front() == nip) {
            antrian.pop();
            n->data.status = "Cuti";
            cout << "Pengajuan cuti berikutnya langsung diproses dari antrian\n";
        }
    }
}

void hapusKaryawan(int nip) {
    if (cari(root, nip) == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }
    root = hapus(root, nip);
    cout << "Data karyawan berhasil dihapus\n";
}

void undo() {
    if (riwayat.empty()) {
        cout << "Tidak ada aksi untuk di-undo\n";
        return;
    }
    auto aksi = riwayat.top(); riwayat.pop();
    Node* n = cari(root, aksi.first);
    if (n == NULL) return;

    if (aksi.second == "Pengajuan") {
        n->data.status = "Tidak Cuti";
        cout << "Undo: Status cuti karyawan dibatalkan\n";
    } else if (aksi.second == "Selesai") {
        n->data.status = "Cuti";
        cout << "Undo: Status cuti karyawan dikembalikan menjadi cuti\n";
    } else if (aksi.second == "Antrian") {
        if (!antrian.empty()) antrian.pop();
        cout << "Undo: Pengajuan cuti dalam antrian dibatalkan\n";
    }
}

void tampilAntrian() {
    if (antrian.empty()) {
        cout << "Antrian kosong\n";
    } else {
        cout << "Karyawan dengan NIP " << antrian.front() << " menunggu giliran cuti\n";
    }
}

int main() {
    int pilihan;
    do {
        cout << "\n=== MENU SISTEM CUTI ===\n";
        cout << "1. Tambah Karyawan\n";
        cout << "2. Edit Karyawan\n";
        cout << "3. Tampil Karyawan\n";
        cout << "4. Ajukan Cuti\n";
        cout << "5. Selesai Cuti\n";
        cout << "6. Hapus Karyawan\n";
        cout << "7. Undo\n";
        cout << "8. Tampil Antrian\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        if (pilihan == 1) {
            int nip; string nama, divisi;
            cout << "Masukkan NIP: "; cin >> nip;
            cout << "Masukkan Nama: "; cin >> nama;
            cout << "Masukkan Divisi: "; cin >> divisi;
            tambahKaryawan(nip, nama, divisi);
        } else if (pilihan == 2) {
            int nip; string nama, divisi;
            cout << "Masukkan NIP: "; cin >> nip;
            cout << "Masukkan Nama Baru: "; cin >> nama;
            cout << "Masukkan Divisi Baru: "; cin >> divisi;
            editKaryawan(nip, nama, divisi);
        } else if (pilihan == 3) {
            tampilKaryawan();
        } else if (pilihan == 4) {
            int nip; cout << "Masukkan NIP: "; cin >> nip;
            ajukanCuti(nip);
        } else if (pilihan == 5) {
            int nip; cout << "Masukkan NIP: "; cin >> nip;
            selesaiCuti(nip);
        } else if (pilihan == 6) {
            int nip; cout << "Masukkan NIP: "; cin >> nip;
            hapusKaryawan(nip);
        } else if (pilihan == 7) {
            undo();
        } else if (pilihan == 8) {
            tampilAntrian();
        }
    } while (pilihan != 0);

    return 0;
}

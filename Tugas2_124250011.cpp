#include <iostream>
#include <iomanip>
using namespace std;

// ================= DATA KARYAWAN =================
struct Karyawan {
    int nip;
    string nama;
    string divisi;
    string status;
};

// ================= BST =================
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

struct Riwayat {
    int nip;
    string aksi;
    Riwayat* next;
};

Riwayat* topRiwayat = NULL;

void pushRiwayat(int nip, string aksi) {
    Riwayat* baru = new Riwayat;
    baru->nip = nip;
    baru->aksi = aksi;
    baru->next = topRiwayat;
    topRiwayat = baru;
}

bool riwayatKosong() {
    return topRiwayat == NULL;
}

Riwayat popRiwayat() {
    Riwayat data = *topRiwayat;
    Riwayat* hapus = topRiwayat;
    topRiwayat = topRiwayat->next;
    delete hapus;
    return data;
}

struct Antrian {
    int nip;
    Antrian* next;
};

Antrian* frontAntrian = NULL;
Antrian* rearAntrian = NULL;

void enqueue(int nip) {
    Antrian* baru = new Antrian;
    baru->nip = nip;
    baru->next = NULL;

    if (frontAntrian == NULL) {
        frontAntrian = rearAntrian = baru;
    } else {
        rearAntrian->next = baru;
        rearAntrian = baru;
    }
}

void dequeue() {
    if (frontAntrian == NULL) return;

    Antrian* hapus = frontAntrian;
    frontAntrian = frontAntrian->next;

    if (frontAntrian == NULL) {
        rearAntrian = NULL;
    }

    delete hapus;
}

bool antrianKosong() {
    return frontAntrian == NULL;
}

int frontQueue() {
    return frontAntrian->nip;
}

// ================= BST FUNCTION =================
Node* insert(Node* node, Karyawan k) {
    if (node == NULL) return new Node(k);

    if (k.nip < node->data.nip) {
        node->left = insert(node->left, k);
    } else if (k.nip > node->data.nip) {
        node->right = insert(node->right, k);
    }

    return node;
}

Node* cari(Node* node, int nip) {
    if (node == NULL) return NULL;

    if (nip == node->data.nip) return node;

    if (nip < node->data.nip) {
        return cari(node->left, nip);
    }

    return cari(node->right, nip);
}

Node* cariMinimum(Node* node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }

    return node;
}

Node* hapus(Node* node, int nip) {
    if (node == NULL) return NULL;

    if (nip < node->data.nip) {
        node->left = hapus(node->left, nip);
    } else if (nip > node->data.nip) {
        node->right = hapus(node->right, nip);
    } else {
        if (node->left == NULL && node->right == NULL) {
            delete node;
            return NULL;
        } else if (node->left == NULL) {
            Node* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == NULL) {
            Node* temp = node->left;
            delete node;
            return temp;
        } else {
            Node* pengganti = cariMinimum(node->right);
            node->data = pengganti->data;
            node->right = hapus(node->right, pengganti->data.nip);
        }
    }

    return node;
}

// ================= OUTPUT =================
void garisMenu() {
    cout << "=====================================\n";
}

void garisPendek() {
    cout << "-------------------------------------\n";
}

void garisTabel() {
    cout << "+----------+---------------+---------------+---------------+\n";
}

void pesanBerhasil(string pesan) {
    cout << "\n[BERHASIL] " << pesan << endl;
}

void pesanGagal(string pesan) {
    cout << "\n[GAGAL] " << pesan << endl;
}

void pesanInfo(string pesan) {
    cout << "\n[INFO] " << pesan << endl;
}

void judulHalaman(string judul) {
    cout << "\n";
    garisPendek();
    cout << "        " << judul << endl;
    garisPendek();
}

void tampilMenu() {
    cout << "\n";
    garisMenu();
    cout << "        SISTEM PENGAJUAN CUTI        \n";
    garisMenu();
    cout << " 1. Tambah Karyawan\n";
    cout << " 2. Edit Karyawan\n";
    cout << " 3. Tampilkan Data Karyawan\n";
    cout << " 4. Ajukan Cuti\n";
    cout << " 5. Selesaikan Cuti\n";
    cout << " 6. Hapus Karyawan\n";
    cout << " 7. Undo Aksi Terakhir\n";
    cout << " 8. Tampilkan Antrian Cuti\n";
    cout << " 0. Keluar\n";
    garisMenu();
    cout << "Pilih menu: ";
}

bool kembaliKeMenu() {
    char pilihan;

    cout << "\n";
    garisPendek();
    cout << "Kembali ke menu utama? (y/n): ";
    cin >> pilihan;

    if (pilihan == 'y' || pilihan == 'Y') {
        return true;
    }

    return false;
}

// ================= TAMPIL DATA =================
void tampil(Node* node) {
    if (node != NULL) {
        tampil(node->left);

        cout << "| " << setw(8) << node->data.nip
             << " | " << setw(13) << node->data.nama
             << " | " << setw(13) << node->data.divisi
             << " | " << setw(13) << node->data.status
             << " |\n";

        garisTabel();

        tampil(node->right);
    }
}

void tampilKaryawan() {
    judulHalaman("DAFTAR DATA KARYAWAN");

    if (root == NULL) {
        pesanInfo("Belum ada data karyawan.");
        return;
    }

    garisTabel();

    cout << "| " << setw(8) << "NIP"
         << " | " << setw(13) << "Nama"
         << " | " << setw(13) << "Divisi"
         << " | " << setw(13) << "Status"
         << " |\n";

    garisTabel();

    tampil(root);
}

// ================= FITUR =================
void tambahKaryawan(int nip, string nama, string divisi) {
    if (cari(root, nip) != NULL) {
        pesanGagal("NIP sudah terdaftar.");
        return;
    }

    Karyawan k = {nip, nama, divisi, "Tidak Cuti"};
    root = insert(root, k);

    pesanBerhasil("Data karyawan berhasil ditambahkan.");
}

void editKaryawan(int nip, string namaBaru, string divisiBaru) {
    Node* n = cari(root, nip);

    if (n == NULL) {
        pesanGagal("Karyawan tidak ditemukan.");
        return;
    }

    n->data.nama = namaBaru;
    n->data.divisi = divisiBaru;

    pesanBerhasil("Data karyawan berhasil diedit.");
}

void ajukanCuti(int nip) {
    Node* n = cari(root, nip);

    if (n == NULL) {
        pesanGagal("Karyawan tidak ditemukan.");
        return;
    }

    if (n->data.status == "Tidak Cuti") {
        n->data.status = "Cuti";
        pushRiwayat(nip, "Pengajuan");

        pesanBerhasil("Pengajuan cuti berhasil diproses.");
    } else {
        enqueue(nip);
        pushRiwayat(nip, "Antrian");

        pesanInfo("Karyawan sedang cuti, pengajuan masuk ke antrian.");
    }
}

void selesaiCuti(int nip) {
    Node* n = cari(root, nip);

    if (n == NULL) {
        pesanGagal("Karyawan tidak ditemukan.");
        return;
    }

    if (n->data.status != "Cuti") {
        pesanGagal("Karyawan tidak sedang cuti.");
        return;
    }

    n->data.status = "Tidak Cuti";
    pushRiwayat(nip, "Selesai");

    pesanBerhasil("Cuti berhasil diselesaikan.");

    if (!antrianKosong() && frontQueue() == nip) {
        dequeue();
        n->data.status = "Cuti";

        pesanInfo("Pengajuan berikutnya langsung diproses dari antrian.");
    }
}

void hapusKaryawan(int nip) {
    if (cari(root, nip) == NULL) {
        pesanGagal("Karyawan tidak ditemukan.");
        return;
    }

    root = hapus(root, nip);

    pesanBerhasil("Data karyawan berhasil dihapus.");
}

void undo() {
    if (riwayatKosong()) {
        pesanInfo("Tidak ada aksi untuk di-undo.");
        return;
    }

    Riwayat aksi = popRiwayat();
    Node* n = cari(root, aksi.nip);

    if (n == NULL) {
        pesanGagal("Data karyawan tidak ditemukan untuk undo.");
        return;
    }

    if (aksi.aksi == "Pengajuan") {
        n->data.status = "Tidak Cuti";
        pesanBerhasil("Undo pengajuan cuti berhasil.");
    } else if (aksi.aksi == "Selesai") {
        n->data.status = "Cuti";
        pesanBerhasil("Undo selesai cuti berhasil.");
    } else if (aksi.aksi == "Antrian") {
        if (!antrianKosong()) {
            dequeue();
        }

        pesanBerhasil("Undo antrian berhasil.");
    }
}

void tampilAntrian() {
    judulHalaman("ANTRIAN CUTI");

    if (antrianKosong()) {
        pesanInfo("Antrian cuti kosong.");
    } else {
        cout << "Karyawan dengan NIP "
             << frontQueue()
             << " sedang menunggu giliran cuti.\n";
    }
}

// ================= MAIN =================
int main() {
    int pilihan;
    bool lanjut = true;

    do {
        tampilMenu();
        cin >> pilihan;

        if (pilihan == 1) {
            int nip;
            string nama, divisi;

            judulHalaman("TAMBAH DATA KARYAWAN");

            cout << "Masukkan NIP    : ";
            cin >> nip;

            cout << "Masukkan Nama   : ";
            cin >> nama;

            cout << "Masukkan Divisi : ";
            cin >> divisi;

            tambahKaryawan(nip, nama, divisi);
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 2) {
            int nip;
            string nama, divisi;

            judulHalaman("EDIT DATA KARYAWAN");

            cout << "Masukkan NIP         : ";
            cin >> nip;

            cout << "Masukkan Nama Baru   : ";
            cin >> nama;

            cout << "Masukkan Divisi Baru : ";
            cin >> divisi;

            editKaryawan(nip, nama, divisi);
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 3) {
            tampilKaryawan();
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 4) {
            int nip;

            judulHalaman("PENGAJUAN CUTI");

            cout << "Masukkan NIP: ";
            cin >> nip;

            ajukanCuti(nip);
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 5) {
            int nip;

            judulHalaman("SELESAIKAN CUTI");

            cout << "Masukkan NIP: ";
            cin >> nip;

            selesaiCuti(nip);
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 6) {
            int nip;

            judulHalaman("HAPUS DATA KARYAWAN");

            cout << "Masukkan NIP: ";
            cin >> nip;

            hapusKaryawan(nip);
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 7) {
            judulHalaman("UNDO AKSI TERAKHIR");

            undo();
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 8) {
            tampilAntrian();
            lanjut = kembaliKeMenu();
        } 
        else if (pilihan == 0) {
            pesanInfo("Program selesai. Terima kasih.");
            lanjut = false;
        } 
        else {
            pesanGagal("Pilihan menu tidak valid.");
            lanjut = kembaliKeMenu();
        }

    } while (lanjut);

    return 0;
}

#include <iostream>
#include <iomanip>
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

// ===== STACK MANUAL =====
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

// ===== QUEUE MANUAL =====
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

    if (rearAntrian == NULL) {
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

    if (frontAntrian == NULL)
        rearAntrian = NULL;

    delete hapus;
}

bool antrianKosong() {
    return frontAntrian == NULL;
}

int frontQueue() {
    return frontAntrian->nip;
}

// ===== BST =====
Node* root = NULL;

Node* insert(Node* node, Karyawan k) {
    if (node == NULL)
        return new Node(k);

    if (k.nip < node->data.nip)
        node->left = insert(node->left, k);

    else if (k.nip > node->data.nip)
        node->right = insert(node->right, k);

    return node;
}

Node* cari(Node* node, int nip) {
    if (node == NULL)
        return NULL;

    if (nip == node->data.nip)
        return node;

    if (nip < node->data.nip)
        return cari(node->left, nip);

    return cari(node->right, nip);
}

Node* hapus(Node* node, int nip) {
    if (node == NULL)
        return NULL;

    if (nip < node->data.nip)
        node->left = hapus(node->left, nip);

    else if (nip > node->data.nip)
        node->right = hapus(node->right, nip);

    else {
        if (node->left == NULL)
            return node->right;

        else if (node->right == NULL)
            return node->left;
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
             << " | " << setw(13) << node->data.status
             << " |" << endl;

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
         << " | " << setw(13) << "Status"
         << " |" << endl;

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

        pushRiwayat(nip, "Pengajuan");

        cout << "Pengajuan cuti berhasil\n";
    }
    else {
        enqueue(nip);

        pushRiwayat(nip, "Antrian");

        cout << "Karyawan sedang cuti, masuk antrian\n";
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

        pushRiwayat(nip, "Selesai");

        cout << "Cuti selesai\n";

        if (!antrianKosong() && frontQueue() == nip) {
            dequeue();

            n->data.status = "Cuti";

            cout << "Antrian berikutnya diproses\n";
        }
    }
}

void hapusKaryawan(int nip) {
    if (cari(root, nip) == NULL) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }

    root = hapus(root, nip);

    cout << "Data berhasil dihapus\n";
}

void undo() {
    if (riwayatKosong()) {
        cout << "Tidak ada aksi undo\n";
        return;
    }

    Riwayat aksi = popRiwayat();

    Node* n = cari(root, aksi.nip);

    if (n == NULL)
        return;

    if (aksi.aksi == "Pengajuan") {
        n->data.status = "Tidak Cuti";

        cout << "Undo pengajuan cuti berhasil\n";
    }

    else if (aksi.aksi == "Selesai") {
        n->data.status = "Cuti";

        cout << "Undo selesai cuti berhasil\n";
    }

    else if (aksi.aksi == "Antrian") {
        dequeue();

        cout << "Undo antrian berhasil\n";
    }
}

void tampilAntrian() {
    if (antrianKosong()) {
        cout << "Antrian kosong\n";
    }
    else {
        cout << "NIP "
             << frontQueue()
             << " sedang menunggu antrian cuti\n";
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
        cout << "Pilih Menu: ";
        cin >> pilihan;

        if (pilihan == 1) {
            int nip;
            string nama, divisi;

            cout << "Masukkan NIP: ";
            cin >> nip;

            cout << "Masukkan Nama: ";
            cin >> nama;

            cout << "Masukkan Divisi: ";
            cin >> divisi;

            tambahKaryawan(nip, nama, divisi);
        }

        else if (pilihan == 2) {
            int nip;
            string nama, divisi;

            cout << "Masukkan NIP: ";
            cin >> nip;

            cout << "Masukkan Nama Baru: ";
            cin >> nama;

            cout << "Masukkan Divisi Baru: ";
            cin >> divisi;

            editKaryawan(nip, nama, divisi);
        }

        else if (pilihan == 3) {
            tampilKaryawan();
        }

        else if (pilihan == 4) {
            int nip;

            cout << "Masukkan NIP: ";
            cin >> nip;

            ajukanCuti(nip);
        }

        else if (pilihan == 5) {
            int nip;

            cout << "Masukkan NIP: ";
            cin >> nip;

            selesaiCuti(nip);
        }

        else if (pilihan == 6) {
            int nip;

            cout << "Masukkan NIP: ";
            cin >> nip;

            hapusKaryawan(nip);
        }

        else if (pilihan == 7) {
            undo();
        }

        else if (pilihan == 8) {
            tampilAntrian();
        }

    } while (pilihan != 0);

    return 0;
}

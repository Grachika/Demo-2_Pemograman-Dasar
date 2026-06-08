#include <stdio.h>
#include <string.h>

// 1. Desain Struct
struct Kendaraan {
    char plat[50];
    char jenis[20]; // "Motor" atau "Mobil"
    int jamMasuk;
    int jamKeluar;
    int biaya;
};

// 2. Fungsi Proses (Menghitung biaya & update via pointer)
void hitungBiaya(struct Kendaraan *k) {
    int durasi = k->jamKeluar - k->jamMasuk;
    if (durasi <= 0) durasi = 1; // Minimal hitungan 1 jam jika jam sama
    
    // Mengecek apakah jenisnya mobil (mengabaikan case sensitif secara manual)
    if (strcmp(k->jenis, "Mobil") == 0 || strcmp(k->jenis, "mobil") == 0) {
        k->biaya = durasi * 5000;
    } else {
        k->biaya = durasi * 2000;
    }
}

// 3. Fungsi Input Data
void inputData(struct Kendaraan *arrKendaraan, int *jumlah) {
    printf("\n--- Input Data Kendaraan ---\n");
    printf("Plat Nomor  : "); 
    // PERBAIKAN: Menggunakan %[^\n] agar bisa membaca spasi
    scanf(" %[^\n]", arrKendaraan[*jumlah].plat);
    
    printf("Jenis (Mobil/Motor): "); 
    scanf(" %s", arrKendaraan[*jumlah].jenis);
    
    printf("Jam Masuk (0-24)   : "); 
    scanf("%d", &arrKendaraan[*jumlah].jamMasuk);
    
    printf("Jam Keluar (0-24)  : "); 
    scanf("%d", &arrKendaraan[*jumlah].jamKeluar);
    
    // Update data biaya melalui pointer
    hitungBiaya(&arrKendaraan[*jumlah]);
    
    (*jumlah)++; // Menambah total kendaraan yang terdaftar
    printf("Data berhasil ditambahkan!\n");
}

// 4. Fungsi Tampilkan Data
void tampilkanData(struct Kendaraan *arrKendaraan, int jumlah) {
    printf("\n--- Data Parkir Kendaraan ---\n");
    if (jumlah == 0) {
        printf("Belum ada data parkir.\n");
        return;
    }
    for (int i = 0; i < jumlah; i++) {
        printf("%d. Plat: %s | Jenis: %s | Masuk: %d | Keluar: %d | Biaya: Rp%d\n", 
               i+1, arrKendaraan[i].plat, arrKendaraan[i].jenis, 
               arrKendaraan[i].jamMasuk, arrKendaraan[i].jamKeluar, arrKendaraan[i].biaya);
    }
}

// 5. Fitur Sorting (Bubble Sort berdasarkan Plat Nomor)
void urutkanData(struct Kendaraan *arrKendaraan, int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (strcmp(arrKendaraan[j].plat, arrKendaraan[j+1].plat) > 0) {
                // Swap struct
                struct Kendaraan temp = arrKendaraan[j];
                arrKendaraan[j] = arrKendaraan[j+1];
                arrKendaraan[j+1] = temp;
            }
        }
    }
    printf("\nData berhasil diurutkan berdasarkan Plat Nomor (A-Z).\n");
}

// 6. Fitur Searching (Linear Search berdasarkan Plat Nomor)
void cariData(struct Kendaraan *arrKendaraan, int jumlah, char cariPlat[]) {
    int ditemukan = 0;
    printf("\n--- Hasil Pencarian ---\n");
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(arrKendaraan[i].plat, cariPlat) == 0) {
            printf("Kendaraan Ditemukan!\n");
            printf("Plat: %s | Jenis: %s | Biaya: Rp%d\n", 
                   arrKendaraan[i].plat, arrKendaraan[i].jenis, arrKendaraan[i].biaya);
            ditemukan = 1;
            break; // Berhenti mencari setelah ditemukan
        }
    }
    if (!ditemukan) {
        printf("Kendaraan dengan plat %s tidak ditemukan.\n", cariPlat);
    }
}

int main() {
    struct Kendaraan dataParkir[100]; // Array of Struct dengan kapasitas 100
    int jumlahKendaraan = 0;          // Menghitung jumlah data saat ini
    int pilihan;
    char cariPlat[50];

    do {
        printf("\n=== SISTEM PARKIR KENDARAAN ===\n");
        printf("1. Input Data Kendaraan\n");
        printf("2. Tampilkan Data\n");
        printf("3. Urutkan Data (Plat Nomor)\n");
        printf("4. Cari Data Kendaraan\n");
        printf("5. Keluar\n");
        printf("Pilih menu: "); 
        scanf("%d", &pilihan);

        switch(pilihan) {
            case 1: 
                inputData(dataParkir, &jumlahKendaraan); 
                break;
            case 2: 
                tampilkanData(dataParkir, jumlahKendaraan); 
                break;
            case 3: 
                urutkanData(dataParkir, jumlahKendaraan); 
                break;
            case 4: 
                printf("Masukkan Plat Nomor yang dicari: "); 
                // PERBAIKAN: Agar pencarian juga mendukung input spasi
                scanf(" %[^\n]", cariPlat);
                cariData(dataParkir, jumlahKendaraan, cariPlat); 
                break;
            case 5: 
                printf("Program selesai. Terima kasih!\n"); 
                break;
            default: 
                printf("Pilihan tidak valid!\n");
        }
    } while(pilihan != 5);

    return 0;
}
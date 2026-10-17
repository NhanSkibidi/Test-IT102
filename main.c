#include <stdio.h>
#include <string.h>
#define MAX_SP 100
#define MAX_HD 20
struct Product {
    char id[11];
    char name[41];
    char unit[11];
    float price;
    int quantity;
};
struct BillItem {
    char id[11];
    char name[41];
    float price;
    int quantity;
    float total;
};
int findProduct(struct Product list[], int n, char id[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(list[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}
void addProduct(struct Product list[], int *n) {
    struct Product p;
    if (*n >= MAX_SP) {
        printf("Danh sach da day!\n");
        return;
    }
    printf("Nhap ma san pham (khong duoc la 0): ");
    scanf(" %10s", p.id);
    if (strcmp(p.id, "0") == 0) {
        printf("Ma san pham khong duoc la 0!\n");
        return;
    }
    if (findProduct(list, *n, p.id) != -1) {
        printf("Ma san pham da ton tai!\n");
        return;
    }
    getchar();
    printf("Nhap ten san pham: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';
    printf("Nhap don vi tinh: ");
    scanf(" %10s", p.unit);
    do {
        printf("Nhap don gia (> 0): ");
        if (scanf("%f", &p.price) != 1) {
            printf("Vui long nhap mot so!\n");
            while (getchar() != '\n');
            p.price = 0;
            continue;
        }
        if (p.price <= 0) {
            printf("Don gia phai lon hon 0!\n");
        }
    } while (p.price <= 0);
    do {
        printf("Nhap so luong ton (>= 0): ");
        if (scanf("%d", &p.quantity) != 1) {
            printf("Vui long nhap so nguyen!\n");
            while (getchar() != '\n');
            p.quantity = -1;
            continue;
        }
        if (p.quantity < 0) {
            printf("So luong khong duoc am!\n");
        }
    } while (p.quantity < 0);
    list[*n] = p;
    (*n)++;
    printf("Them san pham thanh cong!\n");
}
void showProducts(struct Product list[], int n) {
    int i;
    if (n == 0) {
        printf("Chua co san pham nao, hay chon 1 truoc.\n");
        return;
    }
    printf("STT", "Ma", "Ten san pham", "Don vi","Don gia", "SL", "Gia tri ton");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-5d %-12s %-40s %-10s %12.0f %8d %15.0f\n",
               i + 1, list[i].id, list[i].name,
               list[i].unit, list[i].price,
               list[i].quantity,
               list[i].price * list[i].quantity);
    }
}
void searchProduct(struct Product list[], int n) {
    char id[11];
    int pos;
    printf("Nhap ma san pham can tim: ");
    scanf(" %10s", id);
    pos = findProduct(list, n, id);
    if (pos == -1) {
        printf("Khong tim thay!\n");
    } else {
        printf("Ma san pham: %s\n", list[pos].id);
        printf("Ten san pham: %s\n", list[pos].name);
        printf("Don vi tinh: %s\n", list[pos].unit);
        printf("Don gia: %.0f VND\n", list[pos].price);
        printf("So luong ton: %d\n", list[pos].quantity);
        printf("Gia tri ton: %.0f VND\n",
               list[pos].price * list[pos].quantity);
    }
}
void updateProduct(struct Product list[], int n) {
    char id[11];
    int pos, addQty;
    float newPrice;
    printf("Nhap ma san pham can cap nhat: ");
    scanf(" %10s", id);
    pos = findProduct(list, n, id);
    if (pos == -1) {
        printf("Khong tim thay!\n");
        return;
    }
    do {
        printf("Nhap don gia moi (> 0): ");
        if (scanf("%f", &newPrice) != 1) {
            printf("Vui long nhap mot so!\n");
            while (getchar() != '\n');
            newPrice = 0;
            continue;
        }
        if (newPrice <= 0) {
            printf("Don gia phai lon hon 0!\n");
        }
    } while (newPrice <= 0);
    do {
        printf("Nhap so luong nhap them (>= 0): ");
        if (scanf("%d", &addQty) != 1) {
            printf("Vui long nhap so nguyen!\n");
            while (getchar() != '\n');
            addQty = -1;
            continue;
        }
        if (addQty < 0) {
            printf("So luong nhap them khong duoc am!\n");
        }
    } while (addQty < 0);
    list[pos].price = newPrice;
    list[pos].quantity += addQty;
    printf("Cap nhat san pham thanh cong!\n");
}
void deleteProduct(struct Product list[], int *n) {
    char id[11];
    int pos, i;
    printf("Nhap ma san pham can xoa: ");
    scanf(" %10s", id);
    pos = findProduct(list, *n, id);
    if (pos == -1) {
        printf("Khong tim thay!\n");
        return;
    }
    for (i = pos; i < *n - 1; i++) {
        list[i] = list[i + 1];
    }
    (*n)--;
    printf("Xoa san pham thanh cong!\n");
}
void sellProducts(struct Product list[], int n,
                  int *billCount, float *revenue) {
    struct BillItem bill[MAX_HD];
    char id[11];
    int pos, qty, count = 0, i, billPos;
    float subtotal = 0, discount, payment;
    while (1) {
        printf("Nhap ma san pham (0 de ket thuc): ");
        scanf(" %10s", id);
        if (strcmp(id, "0") == 0) {
            break;
        }
        pos = findProduct(list, n, id);
        if (pos == -1) {
            printf("Khong tim thay san pham!\n");
            continue;
        }
        if (list[pos].quantity == 0) {
            printf("San pham da het hang!\n");
            continue;
        }
        printf("Nhap so luong mua: ");
        if (scanf("%d", &qty) != 1) {
            printf("So luong khong hop le!\n");
            while (getchar() != '\n');
            continue;
        }
        if (qty <= 0) {
            printf("So luong mua phai lon hon 0!\n");
            continue;
        }
        if (qty > list[pos].quantity) {
            printf("Khong du hang! So luong ton: %d\n",
                   list[pos].quantity);
            continue;
        }
        billPos = -1;
        for (i = 0; i < count; i++) {
            if (strcmp(bill[i].id, id) == 0) {
                billPos = i;
                break;
            }
        }
        if (billPos == -1 && count >= MAX_HD) {
            printf("Hoa don da du 20 dong hang!\n");
            continue;
        }
        if (billPos == -1) {
            billPos = count;
            strcpy(bill[billPos].id, list[pos].id);
            strcpy(bill[billPos].name, list[pos].name);
            bill[billPos].price = list[pos].price;
            bill[billPos].quantity = 0;
            bill[billPos].total = 0;
            count++;
        }
        bill[billPos].quantity += qty;
        bill[billPos].total =
            bill[billPos].quantity * bill[billPos].price;
        list[pos].quantity -= qty;
        subtotal += qty * list[pos].price;
        printf("Da them vao hoa don!\n");
    }
    if (count == 0) {
        printf("Hoa don khong co san pham!\n");
        return;
    }
    printf("================ HOA DON ================\n");
    printf("%-12s %-40s %8s %12s %15s\n",
           "Ma", "Ten san pham", "SL", "Don gia", "Thanh tien");
    for (i = 0; i < count; i++) {
        printf("%-12s %-40s %8d %12.0f %15.0f\n",
               bill[i].id, bill[i].name,
               bill[i].quantity, bill[i].price,
               bill[i].total);
    }
    discount = 0;
    if (subtotal >= 500000) {
        discount = subtotal * 0.1f;
    }
    payment = subtotal - discount;
    printf("---------------------------------------------------------------\n");
    printf("Tong tien hang: %.0f VND\n", subtotal);
    printf("Giam gia: %.0f VND\n", discount);
    printf("Khach phai tra: %.0f VND\n", payment);
    printf("===============================================================\n");
    (*billCount)++;
    *revenue += payment;
}
void lowStockProducts(struct Product list[], int n) {
    int i, count = 0;
    printf("Danh sach san pham sap het hang (ton kho < 5):\n");
    for (i = 0; i < n; i++) {
        if (list[i].quantity < 5) {
            printf("%s - %s - Con %d\n",
                   list[i].id, list[i].name,
                   list[i].quantity);
            count++;
        }
    }
    if (count == 0) {
        printf("Khong co san pham sap het hang!\n");
    }
}
void sortProducts(struct Product list[], int n) {
    int i, j;
    struct Product temp;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (list[i].price > list[j].price) {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    printf("Da sap xep theo don gia tang dan!\n");
    showProducts(list, n);
}
void statistics(struct Product list[], int n,
                int billCount, float revenue) {
    int i, maxPos = 0;
    float stockValue = 0;
    for (i = 0; i < n; i++) {
        stockValue += list[i].price * list[i].quantity;
        if (i == 0 || list[i].price > list[maxPos].price) {
            maxPos = i;
        }
    }
    printf("========== THONG KE ==========\n");
    printf("So mat hang: %d\n", n);
    printf("Tong gia tri ton kho: %.0f VND\n", stockValue);
    if (n > 0) {
        printf("San pham co don gia cao nhat: %s - %.0f VND\n",
               list[maxPos].name, list[maxPos].price);
    } else {
        printf("Chua co san pham de thong ke.\n");
    }
    printf("So hoa don da ban: %d\n", billCount);
    printf("Tong doanh thu: %.0f VND\n", revenue);
    printf("==============================\n");
}
int main(void) {
    struct Product list[MAX_SP];
    int n = 0;
    int choice;
    int billCount = 0;
    float revenue = 0;
    do {
        printf("\n====== QUAN LY CUA HANG ======\n");
        printf("1. Them san pham\n");
        printf("2. Hien thi danh sach san pham\n");
        printf("3. Tim san pham theo ma\n");
        printf("4. Cap nhat gia / nhap them hang\n");
        printf("5. Xoa san pham\n");
        printf("6. Ban hang va in hoa don\n");
        printf("7. Danh sach san pham sap het hang\n");
        printf("8. Sap xep theo don gia tang dan\n");
        printf("9. Thong ke\n");
        printf("0. Thoat\n");
        printf("==============================\n");
        printf("Chon: ");
        if (scanf("%d", &choice) != 1) {
            printf("Vui long nhap mot so tu menu!\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        if (choice >= 2 && choice <= 9 && n == 0) {
            printf("Chua co san pham nao, hay chon 1 truoc.\n");
            continue;
        }
        switch (choice) {
            case 1:
                addProduct(list, &n);
                break;
            case 2:
                showProducts(list, n);
                break;
            case 3:
                searchProduct(list, n);
                break;
            case 4:
                updateProduct(list, n);
                break;
            case 5:
                deleteProduct(list, &n);
                break;
            case 6:
                sellProducts(list, n, &billCount, &revenue);
                break;
            case 7:
                lowStockProducts(list, n);
                break;
            case 8:
                sortProducts(list, n);
                break;
            case 9:
                statistics(list, n, billCount, revenue);
                break;
            case 0:
                printf("Da thoat chuong trinh!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);
    return 0;
}
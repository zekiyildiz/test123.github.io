# Makefile

# Derleyici olarak Clang kullan
CC = clang

# Derleyici bayrakları: Tüm uyarıları etkinleştir ve hata ayıklama bilgisi ekle
CFLAGS = -Wall -g

# Bağımlılık dosyaları
DEPS = player.h room.h item.h creature.h game.h

# Derlenecek obje dosyaları
OBJ = main.o player.o room.o item.o creature.o game.o

# Çıktı dosyası adı
TARGET = dungeon_game

# Varsayılan hedef: tüm projeyi derle
all: $(TARGET)

# Hedef dosyayı oluşturmak için gerekli obje dosyalarını derle
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# .c dosyalarını .o dosyalarına derlemek için kural
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Temizlik hedefi: obje dosyalarını ve çıktıyı sil
.PHONY: clean

clean:
	rm -f *.o $(TARGET)

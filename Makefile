# Makefile

# Derleyici ve bayraklar
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = 

# Kaynak dosyalar ve nesne dosyaları
SRCS = main.c game.c player.c room.c item.c creature.c
OBJS = $(SRCS:.c=.o)

# Yürütülebilir dosya adı
TARGET = game

# Varsayılan hedef: tümünü derle
all: $(TARGET)

# Yürütülebilir dosyanın nasıl oluşturulacağını belirt
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Her bir .c dosyasını .o dosyasına dönüştür
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Projeyi temizlemek için 'make clean' komutu
clean:
	rm -f $(OBJS) $(TARGET)

# 'make run' komutu ile oyunu çalıştırmak
run: $(TARGET)
	./$(TARGET)

# 'make rebuild' komutu ile projeyi temizleyip tekrar derlemek
rebuild: clean all

.PHONY: all clean run rebuild

# Projeto IAM - ESP32-S3

## Compilar com Docker

Com Docker Desktop iniciado, abra o PowerShell nesta pasta e execute:

```powershell
docker compose run --rm esp-idf build
```

Na primeira execução, o Docker baixa a imagem do ESP-IDF. O firmware compilado fica em `build/template-app.bin` e o arquivo ELF em `build/template-app.elf`.

Para limpar os arquivos de compilação e compilar novamente:

```powershell
docker compose run --rm esp-idf fullclean
docker compose run --rm esp-idf build
```

## Simular no Wokwi

Depois de compilar, abra `diagram.json` no Wokwi pelo VS Code e inicie a simulação. O `wokwi.toml` já aponta para os arquivos gerados em `build/`.

O MPU6050 inicia com aceleração X/Y/Z de `0.35`, `-0.20` e `0.92 g`, e rotação X/Y/Z de `12`, `-8` e `25 graus/s`.

<!-- Valores iniciais anteriores do Wokwi: aceleração X/Y/Z = 0, 0, 1 g; rotação X/Y/Z = 0, 0, 0 graus/s. -->

O container acima compila o firmware; a simulação é executada pelo Wokwi. Para gravar em uma placa física pelo Windows, use a extensão ESP-IDF ou configure o acesso USB do Docker separadamente.
import io

def remove_accents(text):
    mapping = {
        'á': 'a', 'à': 'a', 'ã': 'a', 'â': 'a', 'ä': 'a',
        'é': 'e', 'è': 'e', 'ê': 'e', 'ë': 'e',
        'í': 'i', 'ì': 'i', 'î': 'i', 'ï': 'i',
        'ó': 'o', 'ò': 'o', 'õ': 'o', 'ô': 'o', 'ö': 'o',
        'ú': 'u', 'ù': 'u', 'û': 'u', 'ü': 'u',
        'ç': 'c', 'ñ': 'n',
        'Á': 'A', 'À': 'A', 'Ã': 'A', 'Â': 'A', 'Ä': 'A',
        'É': 'E', 'È': 'E', 'Ê': 'E', 'Ë': 'E',
        'Í': 'I', 'Ì': 'I', 'Î': 'I', 'Ï': 'I',
        'Ó': 'O', 'Ò': 'O', 'Õ': 'O', 'Ô': 'O', 'Ö': 'O',
        'Ú': 'U', 'Ù': 'U', 'Û': 'U', 'Ü': 'U',
        'Ç': 'C', 'Ñ': 'N',
        '—': '-', '–': '-',
        '“': '"', '”': '"',
        '‘': "'", '’': "'",
        'º': 'o', 'ª': 'a'
    }
    
    # Substitui caracteres específicos
    for k, v in mapping.items():
        text = text.replace(k, v)
        
    # Troca qualquer outro caractere que não seja ASCII por um espaço 
    result = ""
    for char in text:
        if ord(char) < 128:
            result += char
        else:
            result += ' '
            
    # CORREÇÃO BÔNUS: Resolve a ambiguidade do next() no C++!
    result = result.replace("next()", "pausa()")
    result = result.replace("void next()", "void pausa()")
    
    return result

def main():
    print("Lendo main.cpp...")
    try:
        with io.open("main.cpp", "r", encoding="utf-8") as f:
            content = f.read()
    except UnicodeDecodeError:
        with io.open("main.cpp", "r", encoding="latin-1") as f:
            content = f.read()

    print("Removendo acentos e corrigindo ambiguidades...")
    new_content = remove_accents(content)

    print("Salvando main.cpp convertido...")
    with io.open("main.cpp", "w", encoding="utf-8") as f:
        f.write(new_content)
        
    print("Pronto! Todos os textos agora estao em formato ASCII e os couts corrigidos.")

if __name__ == "__main__":
    main()

printf "######################"
printf "#   COMPIL ARDUINO   #"
printf "######################"

for i in "$@"
do
case $i in
    -f)
        FOLDER=$2
    shift
    shift
    ;;
esac
done

echo $FOLDER
cd $FOLDERls -l

if [ ! -d "$FOLDER" ];then
    echo "Le dossier $FOLDER n'existe pas"
    exit
elif [ "$FOLDER" != ''];then
    printf 'P1'
    cd $FOLDER
    if [ ! -d ".tmp" ];then
        mkdir .tmp/*
    else
        rm -rf .tmp/*
    fi
    printf '.'
    if [ ! -d "build" ];then
        mkdir build
    else
    rm -rf .build/*
    fi
    printf '.'
    printf ' done [env ready]\n'
else
    echo 'Aucun dossier spécifié, indiquer f=<nom_du_dossier>'
    exit
fi

for c in *.c;do
    [ -f "$c" ] || break
    avr-gcc  -0s -DF_CPU=16000000L -mmcu=atmega328p -c $c -o .tmp/${cpp%.*}.o
    filesC="$filesC $c"
done

printf 'P3'
avr-gcc -DF_CPU=16000000L -mmcu=atmega328p $filesC -o build/$FOLDER 2> /dev/null
printf '.done [links]\n'

printf "Voulez vous commencer le téléversement ? [0]:"
read -r TELEV
if [[$TELEV == 'N' ]];then
    printf "Bye bye\n"
    exit

if [ $INIT == 1 ];then
    printf "INITIALISATION EN COURS\n"
    rm $FOLDER/.arduicesi   #à vérifier (possible de remplacer par le nom du dossier du main (src))

    printf 'Quel type de carte utilisez-vous ? [atmega328p]'
    read -r TYPE
    if [[ $TYPE == '' ]];then
        TYPE="atmega328p"
    fi
    echo ' type: "'$TYPE'",' >> $FOLDER/.arduicesi #à vérifier (possible de remplacer par le nom du dossier du main (src))
    printf 'Quel port utilisez-vous ? [/dev/ttyS1] '
    read -r PORT
    if [[ $PORT == '' ]];then
        PORT="/dev/ttyS1"
    fi

    echo ' port: "'$PORT'",' >> $FOLDER/.arduicesi #à vérifier (possible de remplacer par le nom du dossier du main (src))
    echo "}" >> $FOLDER/.arduicesi #à vérifier (possible de remplacer par le nom du dossier du main (src))
fi

#GET CONFIGURATION

CONF_TYPE=$(grep -oP '(?<=type: ")[^"]*' $FOLDER/.arduicesi) #(possible de remplacer par le nom du dossier du main (src))
CONF_PORT=$(grep -oP '(?<=port: ")[^"]*' $FOLDER/.arduicesi) #(possible de remplacer par le nom du dossier du main (src))

#############
# FUNCTIONS #
#############
function cleaInsideFolder(){
    if [ "$1" != "" ];then
        rm -rf $1/*;
    else
        echo "error folder is null"
    fi
}

if [ ! -d "$FOLDER" ];then
    echo "Le dossier $FOLDER n'existe pas"
    exit
elif [ "$FOLDER" != '' ];then
    printf 'P1'
    cd $FOLDER
    if [ ! -d ".tmp" ];then
        mkdir .tmp
    else
        cleaInsideFolder ".tmp/"
    fi
    printf '.'
    if [ ! -d "build" ];then
        mkdir build
    else
        cleaInsideFolder ".build/"
    fi
    printf '.'
    printf ' done [env ready] \n'
else
    echo 'Aucun dossier spécifié, indiquer f=<nom_du_dossier>'
    exit
fi

printf "Clean datas\n"
cleaInsideFolder ".tmp"

DEBUG=$(grep -oP '(?<=#define DEBUG)[^"]*' main.c) #main.cpp (à vérifier)

if [ "$DEBUG" == "1" ]; then
    printf '######### DEBUG MODE ENABLE #########\n'
    if [ ! -d ".debug" ];then
        mkdir .debug
    else
        cleaInsideFolder ".debug/"
    fi

    printf 'Préprocesseur uniquement [/.debug/1preproc/<files.i>]\n'
    if [ ! -d ".debug/1_preproc" ];then
        mkdir .debug/1_preproc
    else
        cleaInsideFolder ".debug/1_preproc/"
    fi
    for c in *.c;do
        printf '.'
        [ -f "$c" ] || break
        avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -E $c -o .debug/1_preproc/${c%.*}.i 2> /dev/null
    done
    printf ' done [.i ready]\n'

    printf 'Compilation uniquement [/.debug/2_compil/<files.s>]\n'
    if [ ! -d ".debug/2_compil" ];then
        mkdir .debug/2_compil
    else
        cleaInsideFolder ".debug/2_compil/"
    fi

    for i in .debug/1_preproc/*.i;do
        printf '.'
        [ -f "$i" ] || break
        printf "File : $i\n"
        avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -S $i -o ${i%.*}.S
    done
    mv .debug/1_preproc/*.s .debug/2_compil
    printf ' done [.s ready]\n'

    printf 'Compilation et Assemblage uniquement [/.debug/3_assemnly/<files.o>]\n'
    if [ ! -d ".debug/3_assembly" ];then
        mkdir .debug/3_assembly
    else
        cleaInsideFolder ".debug/3_assembly/"
    fi

    for i in .debug/1_preproc/*.i;do
        printf '.'
        [ -f "$i" ] || break
        printf "File : $i\n"
        avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -c $i -o ${i%.*}.o
    done
    mv .debug/1_preproc/*.o .debug/3_assembly
    printf ' done [.o ready]\n'
else
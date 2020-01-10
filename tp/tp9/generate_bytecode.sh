SFILE=$1
OUTF=byte_code
echo "Generation du byte_code a partir du fichier "$SFILE 
./progmem/progmem -v -o $OUTF $SFILE

echo "Representation hex du byte code"
od -v -t x1 $OUTF

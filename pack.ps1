# Build JCR

function private:builddate{
        $date = Get-Date
        $ginie = '# Identification data

[Build]
Date=' + $date + '

[ID]
For=Kthura
Sig=893f304d4
'

        echo $ginie > ID/ID.ini
}

pushd
cd Kthura_Assets
builddate
./.pack
popd

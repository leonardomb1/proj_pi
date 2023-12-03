CREATE PROCEDURE [dbo].[alteracaoFolha] @percentual DECIMAL(8,2), @usrid INT

AS

BEGIN
	INSERT INTO TB_FAT_SALFUN (CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT)
	SELECT DISTINCT CE_FUNCIO, VL_SALARI * @percentual, @usrid, NULL
	FROM TB_FAT_SALFUN
	WHERE VF_REGVAL = 1;

	WITH MOV_ANT AS (
		SELECT MIN(ID_REGSAL) AS MIN_ID,CE_FUNCIO
		FROM TB_FAT_SALFUN
		WHERE VF_REGVAL = 1
		GROUP BY CE_FUNCIO
	),

	MOVS AS (
		SELECT ID_REGSAL, movs.CE_FUNCIO, VF_REGVAL
		FROM TB_FAT_SALFUN movs
		INNER JOIN MOV_ANT ant
		ON movs.ID_REGSAL = ant.MIN_ID
	)

	UPDATE MOVS SET VF_REGVAL = 0
END;

CREATE PROCEDURE [dbo].[atualizarValorRegistro]
    @Tabela NVARCHAR(255), -- The name of the table
    @Valor BIT, -- The new value for VF_REGVAL (0 or 1)
    @ColunaFiltro NVARCHAR(255), -- The column to filter on (e.g., CP_FUNCIO or CE_FUNCIO)
    @ValorFiltro NVARCHAR(255) -- The value to filter on
AS
BEGIN
    DECLARE @SqlQuery NVARCHAR(MAX);

    SET @SqlQuery = N'
        BEGIN TRANSACTION;

        UPDATE ' + QUOTENAME(@Tabela) + '
        SET VF_REGVAL = @Valor
        WHERE ' + QUOTENAME(@ColunaFiltro) + ' = @ValorFiltro;

        COMMIT;
    ';

    EXEC sp_executesql @SqlQuery, N'@Valor BIT, @ValorFiltro NVARCHAR(255)', @Valor, @ValorFiltro;
END;

CREATE PROC criaRegistroCargo
	@nome_cargo VARCHAR(30),
	@cbo VARCHAR(6),
	@usuariocriador int
AS
INSERT INTO TB_DIM_CARGO (NM_CARGO, CD_CBO02, CE_USUACR, DT_REGALT, CD_USUAAL)
VALUES(UPPER(@nome_cargo), @cbo, @usuariocriador, NULL, NULL);

CREATE PROC criaRegistroCliente
	@cnpj VARCHAR(14),
	@nome VARCHAR(30),
	@usuariocriador int
AS

INSERT INTO TB_DIM_CLIENT (CP_CLIENT, NM_CLIENT, CE_USUACR, DT_REGALT, CD_USUAAL)
VALUES(@cnpj, UPPER(@nome), @usuariocriador, NULL, NULL);

CREATE PROC [dbo].[criaRegistroFuncionario]
	@cpf VARCHAR(11),
	@nome VARCHAR(30),
	@admissao VARCHAR(8),
	@cargo int,
	@funcao int,
	@dependente int,
	@filial int,
	@depto int,
	@salario float,
	@usuariocriador int

AS

INSERT INTO TB_DIM_FUNCIO (
  CP_FUNCIO, NM_FUNCIO, DT_ADMISS, CE_CARGO, 
  CE_FUNCAO, VL_DEPEND, CE_FILIAL, 
  CE_DEPRTO, CE_USUACR, CD_USUAAL, 
  DT_REGALT
) 
VALUES 
  (
    @cpf, 
    UPPER(@nome), 
    @admissao, 
    @cargo, 
    @funcao, 
    @dependente, 
    @filial, 
    @depto, 
    @usuariocriador, 
    NULL, 
    NULL
  ) INSERT INTO TB_FAT_SALFUN (
    CE_FUNCIO, VL_SALARI, CE_USUACR, CD_USUAAL, 
    DT_REGALT
  ) 
VALUES 
  (@cpf, @salario, @usuariocriador, NULL, NULL);

CREATE PROC criaRegistroVenda
	@cnpj VARCHAR(14),
	@produto int,
	@valor_venda float,
	@qtde_venda int,
	@usuariocriador int

AS

INSERT INTO TB_FAT_VENDA (CE_CLIENT, CE_PRODUT, VL_VENDA, QT_VENDA, CE_USUACR, DT_REGALT, CD_USUAAL)
VALUES(@cnpj, @produto, @valor_venda, @qtde_venda, @usuariocriador, NULL, NULL);

CREATE PROCEDURE [dbo].[invalidaUltMov] @cpf VARCHAR(11)
AS 

BEGIN

WITH ULT_MOV AS (
    SELECT TOP 1 ID_REGSAL, VF_REGVAL
    FROM TB_FAT_SALFUN
    WHERE CE_FUNCIO = @cpf
    ORDER BY ID_REGSAL DESC
)
UPDATE ULT_MOV
SET VF_REGVAL = 0

END;

CREATE PROC movAcrescimoSalario
	@cpf VARCHAR(11),
	@acrescimo float,
	@usuariocriador int

AS

INSERT INTO TB_FAT_SALFUN (
  CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT, CD_USUAAL
) 
SELECT 
  TOP 1 CE_FUNCIO, 
  VL_SALARI + @acrescimo, 
  @usuariocriador, 
  NULL ,
  NULL
FROM 
  TB_FAT_SALFUN 
WHERE 
  CE_FUNCIO = @cpf 
ORDER BY 
  ID_REGSAL DESC;

CREATE PROC movAumentoPercSalario
	@cpf VARCHAR(11),
	@percentual float,
	@usuariocriador int

AS

INSERT INTO TB_FAT_SALFUN (
  CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT, CD_USUAAL
) 
SELECT 
  TOP 1 CE_FUNCIO, 
  VL_SALARI * @percentual, 
  @usuariocriador, 
  NULL ,
  NULL
FROM 
  TB_FAT_SALFUN 
WHERE 
  CE_FUNCIO = @cpf 
ORDER BY 
  ID_REGSAL DESC;

CREATE PROC movimentacaoNovoSalario
	@cpf VARCHAR(11),
	@salario float,
	@cargo int,
	@usuariocriador int

AS

INSERT INTO TB_FAT_SALFUN (CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT, CD_USUAAL) 
VALUES 
  (
    @cpf, @salario, @usuariocriador, NULL, NULL
  );
UPDATE 
  TB_DIM_FUNCIO 
SET 
  CE_CARGO = @cargo, 
  CD_USUAAL = @usuariocriador, 
  DT_REGALT = DEFAULT 
WHERE 
  CP_FUNCIO = @cpf;

CREATE PROC [dbo].[movPromocaoFuncao]
	@cpf VARCHAR(11),
	@salario float,
	@funcao int,
	@usuariocriador int

AS

INSERT INTO TB_FAT_SALFUN (CE_FUNCIO, VL_SALARI, CE_USUACR, DT_REGALT, CD_USUAAL) 
VALUES 
  (
    @cpf, @salario, @usuariocriador, NULL, NULL
  );
UPDATE 
  TB_DIM_FUNCIO 
SET 
  CE_FUNCAO = @funcao, 
  CD_USUAAL = @usuariocriador, 
  DT_REGALT = DEFAULT 
WHERE 
  CP_FUNCIO = @cpf;

CREATE PROC [dbo].[selectNomeAlias]
    @tabela VARCHAR(16),
    @col1 VARCHAR(9),
    @col2 VARCHAR(9),
    @col3 VARCHAR(9),
    @col4 VARCHAR(9),
    @col5 VARCHAR(9),
    @col6 VARCHAR(9),
    @col7 VARCHAR(9),
    @col8 VARCHAR(9)
AS
BEGIN

DECLARE @sqlQuery NVARCHAR(MAX) = N'SELECT ';

SELECT @sqlQuery += COLUMN_NAME + ' AS ' + '[' + TB_META_COLUNAS.NM_NOMETR + ']' + ', '
FROM INFORMATION_SCHEMA.COLUMNS
LEFT JOIN TB_META_COLUNAS ON COLUMN_NAME = NM_COLUNA
WHERE TABLE_NAME = @tabela AND NM_TABELA = @tabela AND COLUMN_NAME IN (@col1,@col2,@col3,@col4,@col5,@col6,@col7,@col8)


SET @sqlQuery = LEFT(@sqlQuery, LEN(@sqlQuery) - 1);

SET @sqlQuery += ' FROM' + QUOTENAME(@tabela) + 'WHERE ' + QUOTENAME(@tabela) + '.VF_REGVAL = 1';


EXEC sp_executesql @sqlQuery;
END;
